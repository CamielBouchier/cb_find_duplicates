//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#include <QDebug>

#include "cb_abort.h"
#include "cb_qfile.h"

//..................................................................................................

cb_qfile::cb_qfile(const QString& name, 
                   const char*    source_file, 
                         int      source_line) 

    : QFile(name)
    {
    }

//..................................................................................................

cb_qfile::cb_qfile(const QString& name, 
                         QObject* parent, 
                   const char*    source_file, 
                         int      source_line) 

    : cb_qfile(name, source_file, source_line)
    {
    setParent(parent);
    }

//..................................................................................................

cb_qfile::cb_qfile(const QString&            name, 
                         QIODevice::OpenMode mode, 
                   const char*               source_file, 
                         int                 source_line) 

    : cb_qfile(name, source_file, source_line)
    {
    if (not open(mode))
        {
        auto error_msg = tr("Could not open '%1' in mode '%2' at '%3:%4'.")
                         .arg(name).arg(mode).arg(source_file).arg(source_line);
        ABORT(error_msg);
        }
    }

//..................................................................................................

cb_qfile::cb_qfile(const QString&            name, 
                         QIODevice::OpenMode mode, 
                         QObject*            parent,
                   const char*               source_file, 
                         int                 source_line) 

    : cb_qfile(name, mode, source_file, source_line)
    {
    setParent(parent);
    }

//..................................................................................................

cb_qfile::~cb_qfile()
    {
    }
 
//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
