//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#pragma once

#include <QApplication>
 
class QSettings;

class cb_find_duplicates : public QApplication 
    {
    Q_OBJECT

    public:
        cb_find_duplicates(int& argc, char* argv[]);
        ~cb_find_duplicates();

        QString                     m_data_location;
        std::unique_ptr <QSettings> m_user_settings;
    private:
    };

//..................................................................................................

extern class cb_find_duplicates* cb_app;

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
