//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#pragma once

#include <QFile>

//..................................................................................................

// Reason subclassing: 
//    check open/close files
//    open with constructor

class cb_qfile : public QFile
    {
    public:
        cb_qfile(const QString& name, 
                       QObject* parent, 
                 const char*    source_file, 
                       int      source_line);
    
        cb_qfile(const QString& name, 
                 const char*    source_file, 
                       int      source_line);
    
        cb_qfile(const QString&            name, 
                       QIODevice::OpenMode mode, 
                       QObject*            parent, 
                 const char*               source_file, 
                       int                 source_line);
    
        cb_qfile(const QString&            name, 
                       QIODevice::OpenMode mode, 
                 const char*               source_file, 
                       int                 source_line);
    
        ~cb_qfile();
    };
 
//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
