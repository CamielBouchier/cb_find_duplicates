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

#include "cb_lua_selector.h"
#include "cb_filesystem_model.h"
#include "cb_main_window.h"
#include "cb_result_model.h"
#include "cb_worker.h"

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

        QString                           m_data_location;
        QFile                             m_failed_logfile;
        QString                           m_temp_location;
        QString                           m_action_success_filename;
        QString                           m_action_fail_filename;
        QString                           m_walk_fail_filename;
        std::unique_ptr <QSettings>       m_user_settings;
        std::unique_ptr <cb_lua_selector> m_lua_selector;
        std::unique_ptr <cb_main_window>  m_main_window;
    private:


        void cb_install_filesystem_model();
        void cb_install_result_model();
        void cb_install_to_data_location();
        void cb_install_worker();
        void cb_launch_main_window();
        void cb_process_args(int& argc, char* argv[]);
        void cb_recursive_copy(const QString& src_dir, const QString& dst_dir);
        void cb_set_data_location();
        void cb_set_stylesheet();
        void cb_set_temp_location();
        void cb_set_user_settings();

        std::unique_ptr <cb_filesystem_model> m_filesystem_model;
        std::unique_ptr <cb_result_model>     m_result_model;
        std::unique_ptr <QThread>             m_worker_thread;
        cb_worker*                            m_worker;
        QHash<QString, QString>               m_constants_in_stylesheet;
        QString                               m_language;
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

extern class cb_find_duplicates* cb_app;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
