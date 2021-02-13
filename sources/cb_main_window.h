//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include "ui_cb_main_window.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class cb_main_window : public QMainWindow, public Ui::cb_main_window 
	{
    Q_OBJECT

    enum cb_config {config_start, config_walking, config_selecting};

    public:

        cb_main_window();
        ~cb_main_window();

    protected:
        void cb_set_config(cb_config config);
        void cb_reset_progress_box();

        void closeEvent(QCloseEvent* event);
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
