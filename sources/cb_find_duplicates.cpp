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

#include <cb_abort.h>
#include <cb_constants.h>
#include <cb_dialog.h>
#include <cb_filesystem_model.h>
#include <cb_find_duplicates.h>
#include <cb_log.h>
#include <cb_main_window.h>
#include <cb_qfile.h>
#include <cb_result_model.h>
#include <cb_support.h>

using namespace std;
// XXX CB TODO
#undef _OPENMP

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
    cb_install_result_model();

    // For the cb_do_gui_communication we have 2 timers.
    // m_ui_elapsed_timer:
    //    That is used primarly when cb_do_gui_communication is called in for loops etc.
    //    It limits the number of gui updates.
    // m_ui_clock_timer:
    //    Fired every 500ms to force cb_do_gui_communication in case no for loop is calling it.

    m_ui_clock_timer.start(500);

    connect(&m_ui_clock_timer, 
            &QTimer::timeout, 
            this, 
            &cb_find_duplicates::cb_do_gui_communication);

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
        ABORT(err_msg);
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
        ABORT(err_msg);
        }

    qInfo() << "m_temp_location:" << m_temp_location;

    m_action_success_filename = m_temp_location + tr("/action_success.log");
    m_action_fail_filename    = m_temp_location + tr("/action_fail.log");
    m_walk_fail_filename      = m_temp_location + tr("/walk_fail.log");
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_recursive_copy(const QString& src_dir, const QString& dst_dir)
    {
    if (not QFileInfo(src_dir).isDir())
        {
        auto err_msg = tr("Fatal: '%1' does not exist.").arg(src_dir);
        ABORT(err_msg);
        }
     if (!QDir().mkpath(dst_dir))
        {
        auto err_msg = tr("Fatal: could not create '%1'.").arg(dst_dir);
        ABORT(err_msg);
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
            ABORT(err_msg);
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
    err_msg += "\n(";
    auto cmd_line = exe_name;
    for (short i=1; i<argc; i++) 
        {
        err_msg += QString(" ") + argv[i];
        }
    err_msg += ")";

    if (argc % 2 != 1) // argc must be odd
        {
        ABORT(err_msg);
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
            ABORT(err_msg);
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
        ABORT(err_msg)
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

void cb_find_duplicates::cb_install_result_model()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_result_model = make_unique<cb_result_model>();
  
    m_main_window->tv_result->setModel(m_result_model.get());
    m_main_window->tv_result->setSortingEnabled(true);
    m_main_window->tv_result->sortByColumn(cb_result_model::column_dirname,
                                           Qt::SortOrder::AscendingOrder);

    for (auto column = 0; column < cb_result_model::nr_columns; column++)
        {
        int default_width = 0;
        switch (column) 
            {
            case cb_result_model::column_basename : default_width = 180; break;
            case cb_result_model::column_dirname  : default_width = 400; break;
            case cb_result_model::column_key      : default_width = 100; break;
            case cb_result_model::column_size     : default_width = 80;  break;
            case cb_result_model::column_mtime    : default_width = 180; break;
            case cb_result_model::column_inode    : default_width = 140; break;
            default : 
                auto err_msg = tr("Unforeseen column (%1).").arg(column);
                ABORT(err_msg);
            }
        QString key = QString("window/result_col_width_%1").arg(column);
        int width = m_user_settings->value(key, default_width).toInt();
        m_main_window->tv_result->setColumnWidth(column, width);
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_find_duplicates::cb_on_start_search()
    {
    qInfo() << __PRETTY_FUNCTION__;

    cb_qfile logfile_fail(m_walk_fail_filename,
                          QIODevice::WriteOnly | QIODevice::Text,
                          __FILE__,__LINE__);

    m_main_window->cb_set_config(cb_main_window::config_walking);

    m_result_model->cb_reset();

    auto dirs_to_handle = m_filesystem_model->cb_get_selected();
    qInfo() << "dirs_to_handle:" << dirs_to_handle;

    auto& key_dict = m_result_model->m_key_dict;
  	key_dict.clear();

    m_ui_elapsed_timer.start();

    auto ui_base_status = tr("Collecting files and their size:");
  	
  	m_walk            = true;
    m_phase           = phase_sizes;
	m_ui_done_files   = 0;
	m_ui_nr_failed    = 0;
  	m_ui_status       = ui_base_status;
  	m_ui_total_files  = 0;
    m_ui_start_time   = QDateTime::currentDateTime();

  	for (auto&& dir : dirs_to_handle)
    	{
    	if (not cb_do_gui_communication()) break;

        // It is important not to take any soft link or shortcut into account.
        // First of all, they are not the ones taking space.
        // Then, removing duplicates would be complex and risky in order not
        // to be left with dangling links. We just avoid the unnecessary risk.

    	QDirIterator dir_iterator(dir, 
                                  QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::Hidden,
                                  QDirIterator::Subdirectories);

    	while (dir_iterator.hasNext())
      		{
    		if (not cb_do_gui_communication()) break;
      	
            auto next = dir_iterator.next();
      		// Additional check/report on unreadable directories.
      		if (QFileInfo(next).isDir())
        		{
        		if (not QFileInfo(next).isReadable())
          			{
          			QTextStream(&logfile_fail) << tr("Not readable directory:")
							                   << next
											   << Qt::endl;
          			m_ui_nr_failed++;
          			}
        		continue;
        		}
      	
            auto file = QFileInfo(next).canonicalFilePath();
            m_ui_status = ui_base_status + " " + file;

      		if (QFileInfo(file).isReadable())
        		{
        		auto key = QString::number(QFileInfo(file).size());
        		if (key_dict.contains(key))
          			{
          			key_dict[key].append(file);
          			}
        		else
          			{
          			key_dict[key] = QStringList(file);
          			}
        		}
      		else
        		{
        		QTextStream(&logfile_fail) << tr("Not readable:")
						                   << file 
										   << Qt::endl;
        		m_ui_nr_failed++;
        		}
       		m_ui_total_files ++;
      		}
    	}

    // m_result_model->cb_log_key_dict();

  	// At this point we have a dictionary key(unique on Size)=>ListOfFiles
  	// Now doing partial and full MD5 calculations to break the tie

    for (m_phase = phase_partial_md5; m_phase <= phase_full_md5; m_phase=cb_phase(m_phase+1)) 
    	{
    	if (not cb_do_gui_communication()) break;

        m_ui_phase_start_time = QDateTime::currentDateTime();
     	m_ui_done_files = 0;

        QString base_status;
		if (m_phase == phase_partial_md5)
			{
			base_status = tr("Calculating partial MD5 for potential duplicates:"); 
			}
		else
			{
            base_status = tr("Calculating full MD5 for potential duplicates:") ; 
			}
  
      	// Remove all entries that don't have at least 2 files.
    	for (auto&& key : key_dict.keys())
      		{
    		if (not cb_do_gui_communication()) break;
      		auto& files = key_dict[key];
      		auto nr_files = files.size();
            if (nr_files < 2)
                {
        		key_dict.remove(key);
                m_ui_total_files -= nr_files;
                }
      		}
  
    	// Start the MD5 calculation
    	auto old_keys = key_dict.keys();
    	#ifdef _OPENMP
      		#pragma omp parallel for default(shared) schedule(dynamic)
    	#endif
    	for (int idx = 0; idx < old_keys.size(); idx++)
      		{
    		if (not cb_do_gui_communication()) continue;
      		QString     key;
      		QStringList files;
      		int         nr_files;
        	#pragma omp critical
        		{
        		key     = old_keys.at(idx);
        		files   = key_dict[key];
        		nr_files = files.size();
        		if (nr_files < 2) 
          			{
          			ABORT(tr("Internal error. nr_files < 2 for key '%1': %2")
						  .arg(key).arg(nr_files));
          			}
        		}

      		// No point in calculating tie if it are all the same files.
      		if (cb_is_same_file(files)) 
        		{
        		continue;
        		}

      		for (auto&& file : files)
        		{
                m_ui_status = ui_base_status + " " + file;

          		auto md5_sum = cb_md5_sum(file, m_phase == phase_partial_md5);
        		auto new_key = key + "_" + md5_sum;
          		#pragma omp critical
         	 		{
          			key_dict.remove(key);
          			if (key_dict.contains(new_key))
            			{
            			key_dict[new_key].append(file);
            			}
          			else
            			{
            			key_dict[new_key] = QStringList(file);
            			}
           			m_ui_done_files++;
          			}
        		}
      		}
  		}
  
  	// Remove keys that don't have at least 2 files.
  	m_ui_status = tr("Cleaning unique files.");
  	for (auto&& key : key_dict.keys())
    	{
    	if (not cb_do_gui_communication()) break;
    	auto& files   = key_dict[key];
    	auto  nr_files = files.size();
    	if (nr_files < 2)
      		{
      		key_dict.remove(key);
      		}
    	}

  	// Done
    // XXX TODO TOT HIER
  	if (not m_walk)
    	{
    	key_dict.clear();
    	}
  	m_ui_status = m_walk ? tr("Done") : tr("Aborted");
    m_result_model->cb_set_result();
    m_main_window->cb_set_config(cb_main_window::config_selecting);
  
  	m_walk = false;
  	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_find_duplicates::cb_do_gui_communication()
	{
    if (m_ui_elapsed_timer.elapsed() < cb_constants::ui::poll_time)
        {
	    return m_walk;
        }

    m_main_window->statusbar->showMessage(m_ui_status);
    m_main_window->lb_walk_fail_value->setText(QString().setNum(m_ui_nr_failed));
    
    switch (m_phase)
        {
        case phase_sizes:

            m_main_window->lb_done_files->setText("");
            m_main_window->lb_total_files->setText("");
            
            m_main_window->pb_files->setValue(0);
            m_main_window->pb_files->setVisible(false);

            m_main_window->lb_time->setText("");
            m_main_window->lb_time_value->setText("");

            m_main_window->lb_overhead->setText("");
            m_main_window->lb_overhead_value->setText("");

            m_main_window->lb_extra->setText(tr("Number of files found:"));
            m_main_window->lb_extra_value->setText(QString().number(m_ui_total_files));

            break;

        case phase_partial_md5:
        case phase_full_md5:

            {
            m_main_window->lb_done_files->setText(QString().number(m_ui_done_files));
            m_main_window->lb_slash->setText("/");
            m_main_window->lb_total_files->setText(QString().number(m_ui_total_files));

            m_main_window->lb_extra->setText(tr("Number of files to process:"));
            m_main_window->lb_extra_value->setText(QString().number(m_ui_total_files));

            float f_p = (100.0 * m_ui_done_files)/m_ui_total_files;
            int progress_files = isfinite(f_p) ? int(f_p) : 0;

            m_main_window->pb_files->setValue(progress_files);
            m_main_window->pb_files->setVisible(true);

            auto running_seconds = m_ui_phase_start_time.secsTo(QDateTime::currentDateTime());
            float n_s = float(running_seconds) * m_ui_total_files / m_ui_done_files;
            int needed_seconds = isfinite(n_s) ? int(n_s) : 300;
            m_ui_end_time = m_ui_phase_start_time.addSecs(needed_seconds);

            m_main_window->lb_time->setText(tr("Expected end time:"));
            m_main_window->lb_time_value->setText(m_ui_end_time.toString());

            break;
            }

        case phase_done:

            {
            m_main_window->lb_done_files->setText("");
            m_main_window->lb_slash->setText("");
            m_main_window->lb_total_files->setText("");

            m_main_window->pb_files->setVisible(false);

            m_main_window->lb_time->setText(tr("End time:"));
            auto seconds_run = m_ui_start_time.secsTo(m_ui_end_time);
            auto time_run = QTime(0,0).addSecs(seconds_run);
            m_main_window->lb_time_value->setText(
                m_ui_end_time.toString() + " (" + time_run.toString() + " " + tr("runtime") + ")" );
            
            m_main_window->lb_overhead->setText(tr("Overhead bytes:"));
            m_main_window->lb_overhead_value->setText(
                    cb_sizestring_from_size(m_result_model->m_overhead_bytes));

            m_main_window->lb_extra->setText(tr("Nr groups and files:"));
            m_main_window->lb_extra_value->setText(
                      QString::number(m_result_model->m_nr_file_groups) 
                    + " - " 
                    + QString::number(m_result_model->m_total_files)
                    );

            break;
            }

        default:

            auto err_msg = tr("m_phase:").arg(m_phase);
            ABORT(err_msg);
        }

    m_ui_elapsed_timer.restart();       // Restart timer for next call.
    processEvents();                    // Give breathing room to GUI!
    return m_walk;
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
        ABORT(err_msg);
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

    m_walk = false;
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
    
    for (auto column = 0; column < cb_result_model::nr_columns; column++)
        {
        QString key = QString("window/result_col_width_%1").arg(column);
        m_user_settings->setValue(key, m_main_window->tv_result->columnWidth(column));
        }

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
