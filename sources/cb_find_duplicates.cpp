//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
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

    cout.setf(std::ios::unitbuf);
    cerr.setf(std::ios::unitbuf);

    m_data_location = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    cb_log::init();
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
