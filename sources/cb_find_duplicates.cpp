//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include <iostream>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>

#include <cb_constants.h>
#include <cb_dialog.h>
#include <cb_filesystem_model.h>
#include <cb_find_duplicates.h>
#include <cb_log.h>
#include <cb_main_window.h>

using namespace std;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_find_duplicates* cb_app = nullptr;   // for global referencing, like qApp.

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_find_duplicates::cb_find_duplicates(int& argc, char* argv[]) : QApplication(argc, argv)
    {
    QCoreApplication::setOrganizationName(cb_constants::organization_name);
    QCoreApplication::setOrganizationDomain(cb_constants::domain_name);
    QCoreApplication::setApplicationName(cb_constants::application_name);
    QCoreApplication::setApplicationVersion(cb_constants::application_version);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_init(int& argc, char* argv[])
    {
    cout.setf(std::ios::unitbuf);   // important for msys environments.
    cerr.setf(std::ios::unitbuf);   // important for msys environments.
    cb_set_data_location();         // ensuring a valid m_data_location.
    cb_log::cb_init();              // logging facility, needs m_data_location.
    cb_set_temp_location();         // ensuring a valid m_temp_location.
    cb_set_user_settings();         // ensuring a valid m_user_settings.
    cb_log::cb_clean_logdir();      // clean_logdir, needs m_user_settings.
    cb_install_to_data_location();
    cb_process_args(argc, argv);    // needs m_user_settings, needed by set_stylesheet.
    cb_set_stylesheet();
    cb_launch_main_window();
    cb_install_filesystem_model();

    qInfo() << "Starting:" << applicationName() << applicationVersion();
    qInfo() << "Qt version:" << qVersion();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_set_user_settings()
    {
    qInfo() << __PRETTY_FUNCTION__;
    auto filename = m_data_location + "/" + cb_constants::application_name + ".ini";
    m_user_settings = make_unique <QSettings>(filename, QSettings::IniFormat);
    qInfo() << "m_user_settings:" << m_user_settings->fileName();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_set_data_location()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_data_location = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (!QDir().mkpath(m_data_location))
        {
        auto err_msg = tr("Fatal: could not create '%1'").arg(m_data_location);
        qCritical() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }

    qInfo() << "m_data_location:" << m_data_location;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_set_temp_location()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_temp_location = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    m_temp_location += "/";
    m_temp_location += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz");
    m_temp_location += "_";
    m_temp_location += cb_constants::application_name;

    if (!QDir().mkpath(m_temp_location))
        {
        auto err_msg = tr("Fatal: could not create '%1'").arg(m_temp_location);
        qCritical() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }

    qInfo() << "m_temp_location:" << m_temp_location;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_recursive_copy(const QString& src_dir, const QString& dst_dir)
    {
    if (not QFileInfo(src_dir).isDir())
        {
        auto err_msg = tr("Fatal: '%1' does not exist.").arg(src_dir);
        qCritical().noquote() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }
     if (!QDir().mkpath(dst_dir))
        {
        auto err_msg = tr("Fatal: could not create '%1'.").arg(dst_dir);
        qCritical().noquote() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }
    for (auto&& filename : QDir(src_dir).entryList(QDir::Files))
        {
        auto src_file = src_dir + "/" + filename;
        auto dst_file = dst_dir + "/" + filename;
        qDebug() << "Copying" << src_file << "=>" << dst_file;
        QFile::remove(dst_file);
        if (!QFile::copy(src_file, dst_file))
            {
            auto err_msg = tr("Fatal: could not copy '%1' => '%2'.").arg(src_file, dst_file);
            qCritical().noquote() << err_msg;
            QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
            abort();
            }
        }
    for (auto&& dirname : QDir(src_dir).entryList(QDir::Dirs))
        {
        if (dirname != "." and dirname != "..")
            {
            cb_recursive_copy(src_dir + "/" + dirname, dst_dir + "/" + dirname);
            }
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_install_to_data_location()
    {
    qInfo() << __PRETTY_FUNCTION__;

    // Do we have a version.txt file that matches ours? If so no install done.
    QFile version_installed_file(m_data_location + "/version.txt");
    auto do_install = false;
    if (!version_installed_file.exists())
        {
        do_install = true;
        qInfo() << "Installing because no version installed.";
        }
    else
        {
        QString version_installed;
        version_installed_file.open(QIODevice::ReadOnly);
        QTextStream(&version_installed_file) >> version_installed;
        version_installed_file.close();
        if (version_installed != cb_constants::application_version)
            {
            do_install = true;
            qInfo() << "Installing because different version installed.";
            }
        }

    // No reason to install ...
    if (!do_install) 
        {
        qInfo() << "No data to install.";
        return;
        }

    // We found out there is a reason to install ...

    QStringList to_copy_list;

    to_copy_list << "about"
                 << "themes" 
                 << "lua_scripts";

    auto base_src_dir = QApplication::applicationDirPath();
    auto base_src_dirname = QDir(base_src_dir).dirName();
    if (base_src_dirname == "debug" or base_src_dirname == "build") // While developing.
        {
        auto q_base_src_dir = QDir(base_src_dir);
        q_base_src_dir.cdUp();
        base_src_dir = q_base_src_dir.canonicalPath();
        }
    qInfo() << "base_src_dir:" << base_src_dir;

    for (auto&& to_copy_dir : to_copy_list)
        {
        auto src_dir = base_src_dir + "/" + to_copy_dir;
        auto dst_dir = m_data_location + "/" + to_copy_dir;
        cb_recursive_copy(src_dir, dst_dir);
        }

    // Remember we installed by setting the 'version.txt' file.
    version_installed_file.open(QIODevice::WriteOnly);
    QTextStream(&version_installed_file) << cb_constants::application_version;
    version_installed_file.close();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_process_args(int& argc, char* argv[])
    {
    qInfo() << __PRETTY_FUNCTION__;

    if (argc == 1) return;  // Do only effort if there are actually args.

    auto exe_name = QFileInfo(argv[0]).fileName();
    auto err_msg  = tr("Usage : %1 " "[-t theme]").arg(exe_name);
    auto cmd_line = exe_name;
    for (short i=1; i<argc; i++) 
        {
        cmd_line += QString(" ") + argv[i];
        }

    auto local_abort = [&]()
        {
        qCritical() << err_msg;
        qCritical() << cmd_line;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg + '\n' + cmd_line);
        abort();
        };

    if (argc % 2 != 1) // argc must be odd
        {
        local_abort();
        }

    short idx = 1;
    while (idx < argc) 
        {
        QString switch_(argv[idx++]);
        QString value(argv[idx++]);
        if (switch_ == "-t") 
            {
            if (!value.endsWith(".cbt")) 
                {
                value.append(".cbt");
                }
            qInfo() << "Setting theme:" << value;
            m_user_settings->setValue("/theme/file", value);
            } 
        else 
            { 
            local_abort();
            }
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_set_stylesheet()
    {
    qInfo() << __PRETTY_FUNCTION__;

    auto default_theme_file = m_data_location + "/themes/default.cbt";
    if (!QFile(default_theme_file).exists())
        {
        auto err_msg = tr("Fatal: '%1' does not exist").arg(default_theme_file);
        qCritical() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }

    auto theme_file = m_data_location + "/themes/" + 
                      m_user_settings->value("/theme/file", default_theme_file).toString();
    if (!QFile(theme_file).exists())
        {
        theme_file = default_theme_file;
        }

    m_user_settings->setValue("/theme/file", QFileInfo(theme_file).fileName());
    qInfo() << "theme_file:" << theme_file;

    m_constants_in_stylesheet.clear();  // clearing ensures we can be called multiple times.

    QFile stylesheet_file(theme_file);
    if (not stylesheet_file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        {
        qWarning() << tr("Could not open themefile:") << theme_file;
        return;
        }

    QTextStream stylesheet_stream(&stylesheet_file);
    QString stylesheet;
    while (not stylesheet_stream.atEnd()) 
        {
        QString line = stylesheet_stream.readLine().trimmed();
        if (line.isEmpty() or line.startsWith(";"))   // comment
            {
            continue;
            }
        if (line.startsWith("@"))   // constant definition
            {
            QStringList definition = line.split("=");
            m_constants_in_stylesheet[definition[0].trimmed()] = definition[1].trimmed();
            continue;
            }
        if (line.contains("@"))     // replacement, rude inefficient algo, but serving purpose.
            {
            QHashIterator <QString,QString> i(m_constants_in_stylesheet);
            while (i.hasNext()) 
                {
                i.next();
                line.replace(i.key(),i.value());
                }
            }
        stylesheet += line + " \n";
        }
    stylesheet_file.close();

    if (m_constants_in_stylesheet.contains("@MANDATORY_STYLE")) 
        {
        setStyle(m_constants_in_stylesheet.value("@MANDATORY_STYLE")); // preset a style.
        }

    // qDebug().noquote() << "stylesheet:" << stylesheet;

    setStyleSheet(stylesheet);  // apply the stylesheet.
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_launch_main_window()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_main_window = make_unique<cb_main_window>();

    QIcon window_icon;
    window_icon.addPixmap(QPixmap(":/cb_find_duplicates/img/cb_find_duplicates_64px.png"));
    m_main_window->setWindowIcon(window_icon);
    m_main_window->setWindowTitle(cb_constants::application_name);
    
    auto&& splitter_state = m_user_settings->value("window/main_splitter").toByteArray();
    m_main_window->main_splitter->restoreState(splitter_state);

    m_main_window->restoreGeometry(m_user_settings->value("window/geometry").toByteArray());
    m_main_window->restoreState(m_user_settings->value("window/state").toByteArray());

    m_main_window->show();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_install_filesystem_model()
    {
    qInfo() << __PRETTY_FUNCTION__;

    // Our custom filesystem_model (with partial selects etc.)
    m_filesystem_model = make_unique<cb_filesystem_model>();
    m_filesystem_model->setFilter(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot | QDir::Hidden);
    m_filesystem_model->setRootPath(QDir::rootPath());

    // Connecting the tv (treeview) directory selector to our filesystem model
    m_main_window->tv_dir_selector->setModel(m_filesystem_model.get());

    // Setting column widths.
    int dir_selector_col0_width = 
        m_user_settings->value("window/dir_selector_col0_width", 400).toInt();
    m_main_window->tv_dir_selector->setColumnWidth(0, dir_selector_col0_width);

    // Hide "Size", "Type" and "Date Modified" in the treeview directory selector.
    m_main_window->tv_dir_selector->setColumnHidden(1, true);
    m_main_window->tv_dir_selector->setColumnHidden(2, true);
    m_main_window->tv_dir_selector->setColumnHidden(3, true);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_start_search()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_failed_logfile.setFileName(m_temp_location + "/failed.log");
    m_failed_logfile.open(QIODevice::WriteOnly | QIODevice::Text);

    m_main_window->cb_set_config(cb_main_window::config_walking);

    // XXX result_model->cb_reset

    auto dirs_to_handle = m_filesystem_model->cb_get_selected();
    qInfo() << "dirs_to_handle:" << dirs_to_handle;
    

    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_stop_search()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_select_by_script()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_update_select_scripts()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_action()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_walk_fail_detail()
    {
    qInfo() << __PRETTY_FUNCTION__;
    QDesktopServices::openUrl(QUrl("file:///" + m_failed_logfile.fileName(), QUrl::TolerantMode));
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_action_success_detail()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_find_duplicates::cb_on_action_fail_detail()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_language()
    {
    qInfo() << __PRETTY_FUNCTION__;
    if (sender() == m_main_window->action_dutch) 
        {
        m_main_window->action_dutch->setChecked(true);
        m_main_window->action_english->setChecked(false);
        m_language = "nl_BE";
        }
    else if (sender() == m_main_window->action_english)
        {
        m_main_window->action_dutch->setChecked(false);
        m_main_window->action_english->setChecked(true);
        m_language = "en_US";
        }
    QMessageBox::information(m_main_window.get(),
                             tr("Please restart"),
                             tr("The language setting will be changed next time you start."));
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_log()
    {
    qInfo() << __PRETTY_FUNCTION__;
    QDesktopServices::openUrl(QUrl(cb_log::m_logfile_name, QUrl::TolerantMode));
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_about()
    {
    qInfo() << __PRETTY_FUNCTION__;

    auto about_filename = m_data_location + "/about/" + tr("about_english.txt");
    if (!QFile(about_filename).exists())
        {
        auto err_msg = tr("Fatal: '%1' does not exist").arg(about_filename);
        qCritical() << err_msg;
        QMessageBox::critical(nullptr, tr("Aborting"), err_msg);
        abort();
        }
    QFile about_file(about_filename);
    about_file.open(QIODevice::ReadOnly);
    auto message = QTextStream(&about_file).readAll();

    message = message.arg(cb_constants::application_name, cb_constants::application_version);
    auto dialog = make_unique<cb_dialog>(message);

    QIcon window_icon;
    window_icon.addPixmap(QPixmap(":/cb_find_duplicates/img/cb_find_duplicates_64px.png"));
    dialog->setWindowIcon(window_icon);
    dialog->setWindowTitle(tr("About"));
    dialog->exec();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_quit()
    {
    qInfo() << __PRETTY_FUNCTION__;
    exit(EXIT_SUCCESS);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_find_duplicates::~cb_find_duplicates()
    {
    qInfo() << __PRETTY_FUNCTION__;
    qInfo() << "Exiting:" << applicationName() << applicationVersion();

    m_user_settings->setValue("window/dir_selector_col0_width", 
                              m_main_window->tv_dir_selector->columnWidth(0));
    m_user_settings->setValue("window/main_splitter", 
                              m_main_window->main_splitter->saveState());
    m_user_settings->setValue("window/geometry",      
                              m_main_window->saveGeometry());
    m_user_settings->setValue("window/state",         
                              m_main_window->saveState());
    m_user_settings->sync();
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

int main(int argc, char** argv)
    {
    cb_app = new cb_find_duplicates(argc, argv);
    cb_app->cb_init(argc, argv);
    cb_app->exec();
    delete cb_app;
    return EXIT_SUCCESS;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
