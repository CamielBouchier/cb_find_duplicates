//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QApplication>
#include <QFile>
#include <QIcon>
 
class QSettings;

class cb_filesystem_model;
class cb_main_window;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class cb_find_duplicates : public QApplication 
    {
    Q_OBJECT

    public:
        cb_find_duplicates(int& argc, char* argv[]);
        ~cb_find_duplicates();

        void cb_init(int& argc, char* argv[]);

        void cb_on_about();
        void cb_on_action();
        void cb_on_action_fail_detail();
        void cb_on_action_success_detail();
        void cb_on_language();
        void cb_on_log();
        void cb_on_quit();
        void cb_on_select_by_script();
        void cb_on_start_search();
        void cb_on_stop_search();
        void cb_on_update_select_scripts();
        void cb_on_walk_fail_detail();

        QString                     m_data_location;
        QFile                       m_failed_logfile;
        QString                     m_temp_location;
        std::unique_ptr <QSettings> m_user_settings;
    private:


        void cb_install_filesystem_model();
        void cb_install_to_data_location();
        void cb_launch_main_window();
        void cb_process_args(int& argc, char* argv[]);
        void cb_recursive_copy(const QString& src_dir, const QString& dst_dir);
        void cb_set_data_location();
        void cb_set_stylesheet();
        void cb_set_temp_location();
        void cb_set_user_settings();

        std::unique_ptr <cb_main_window>      m_main_window;
        std::unique_ptr <cb_filesystem_model> m_filesystem_model;
        QHash<QString, QString>               m_constants_in_stylesheet;
        QString                               m_language;
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

extern class cb_find_duplicates* cb_app;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
