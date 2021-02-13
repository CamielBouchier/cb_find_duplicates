//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include <cb_find_duplicates.h>
#include <cb_constants.h>
#include <cb_log.h>

using namespace std;

//..................................................................................................

cb_find_duplicates* cb_app = nullptr;

cb_find_duplicates::cb_find_duplicates(int& argc, char* argv[]) : QApplication(argc, argv)
    {
    // Can't wait as cb_app is needed early. E.g. for sharing its m_data_location ..
    cb_app = this;

    QCoreApplication::setOrganizationName(cb_constants::organization_name);
    QCoreApplication::setOrganizationDomain(cb_constants::domain_name);
    QCoreApplication::setApplicationName(cb_constants::application_name);
    QCoreApplication::setApplicationVersion(cb_constants::application_version);

    //
    // Important for msys environments.
    //

    cout.setf(std::ios::unitbuf);
    cerr.setf(std::ios::unitbuf);

    // 
    // Standard data_location and its creation. cb_log will need it already.
    //

    m_data_location = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(m_data_location).exists())
        {
        if (!QDir().mkpath(m_data_location))
            {
            auto err_msg = QObject::tr("Fatal: could not create '%1'").arg(m_data_location);
            qCritical() << err_msg;
            QMessageBox::critical(nullptr, QObject::tr("Aborting"), err_msg);
            abort();
            }
        }

    //
    // logging facility.
    // 

    cb_log::init();

    //
    // User settings.
    //

    auto filename = m_data_location + "/" + cb_constants::application_name + ".ini";
    m_user_settings = make_unique <QSettings>(filename, QSettings::IniFormat);
    qInfo() << "m_user_settings:" << m_user_settings->fileName();

    //
    // Clean logdir. We needed the m_user_settings first.
    //

    cb_log::clean_logdir();

    //
    //
    //

    install_to_data_location();

   

    qInfo() << "Starting" << applicationName() << applicationVersion();
    qInfo() << "Qt version:" << qVersion();

    // qILog() << "Start QT Version:" << QT_VERSION;
    // qFatal("This is fatal!");
    // qILog() << "Start QT Version:" << qVersion();

    }

//..................................................................................................

void cb_find_duplicates::install_to_data_location()
    {
    qInfo() << __PRETTY_FUNCTION__;

    // Do we have a version.txt file that matches ours? If so no install done.
    QFile version_installed_file(m_data_location + "/version.txt");
    auto do_install = false;
    if (!version_installed_file.exists())
        {
        do_install = true;
        qInfo() << "Installing because no version installed";
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
            qInfo() << "Installing because different version installed";
            }
        }

    // No reason to install ...
    if (!do_install) return;

    // We found out there is a reason to install ...
    auto to_copy_list = QStringList() << "themes" << "lua_scripts";
    for (auto&& to_copy_dir : to_copy_list)
        {
        auto src_dir = QApplication::applicationDirPath() + "/" + to_copy_dir;
        auto dst_dir = m_data_location + "/" + to_copy_dir;
        if (!QDir(src_dir).exists())
            {
            auto err_msg = QObject::tr("Fatal: '%1' does not exist").arg(src_dir);
            qCritical() << err_msg;
            QMessageBox::critical(nullptr, QObject::tr("Aborting"), err_msg);
            abort();
            }
        if (!QDir().mkpath(dst_dir))
            {
            auto err_msg = QObject::tr("Fatal: could not create '%1'").arg(dst_dir);
            qCritical() << err_msg;
            QMessageBox::critical(nullptr, QObject::tr("Aborting"), err_msg);
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
                auto err_msg = 
                    QObject::tr("Fatal: could not copy '%1' => '%2'").arg(src_file, dst_file);
                qCritical() << err_msg;
                QMessageBox::critical(nullptr, QObject::tr("Aborting"), err_msg);
                abort();
                }
            }
        }

    // Remember we installed through the 'version.txt' file.
    version_installed_file.open(QIODevice::WriteOnly);
    QTextStream(&version_installed_file) << cb_constants::application_version;
    version_installed_file.close();
    }

//..................................................................................................

cb_find_duplicates::~cb_find_duplicates()
    {
    qInfo() << "Exiting" << applicationName() << applicationVersion();
    }

//..................................................................................................

int main(int argc, char** argv)
    {
    new cb_find_duplicates(argc, argv);
    delete cb_app;
    return EXIT_SUCCESS;
    }

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
