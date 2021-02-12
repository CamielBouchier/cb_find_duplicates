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
            QTextStream(stderr) << "Fatal: could not create '" 
                                << m_data_location 
                                << "'" 
                                << Qt::endl;
            exit(EXIT_FAILURE);
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

    qInfo() << "Starting" << applicationName() << applicationVersion();
    qInfo() << "Qt version:" << qVersion();

    // qILog() << "Start QT Version:" << QT_VERSION;
    // qFatal("This is fatal!");
    // qILog() << "Start QT Version:" << qVersion();

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
