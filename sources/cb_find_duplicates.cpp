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

#include <cb_find_duplicates.h>
#include <cb_constants.h>
#include <cb_log.h>

using namespace std;

//..................................................................................................

cb_find_duplicates::cb_find_duplicates(int& argc, char* argv[]) : QApplication(argc, argv)
    {
    QCoreApplication::setOrganizationName(cb_constants::organization_name);
    QCoreApplication::setOrganizationDomain(cb_constants::domain_name);
    QCoreApplication::setApplicationName(cb_constants::application_name);
    QCoreApplication::setApplicationVersion(cb_constants::application_version);

    cout.setf(std::ios::unitbuf);
    cerr.setf(std::ios::unitbuf);

    cb_log::init();

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
    auto app = make_unique <cb_find_duplicates> (argc, argv);
    return EXIT_SUCCESS;
    }

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
