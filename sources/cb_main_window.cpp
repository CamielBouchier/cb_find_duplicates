//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


#include <QCloseEvent>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "cb_find_duplicates.h"
#include "cb_main_window.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_main_window::cb_main_window() : QMainWindow(nullptr)
    {
    setupUi(this);

    // All ready for translation, but currently no motivation. Remove associated menu item.
    menubar->removeAction(menuLanguage->menuAction());

    // GB DirSelector
    
    gb_dir_selector->setTitle(tr("Directory selection"));

    // GB StartStop

    gb_start_stop->setTitle(tr("Start/Stop duplicate search"));

    tb_start_search->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    tb_start_search->setToolTip(tr("Start search for duplicates"));

    tb_stop_search->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    tb_stop_search->setToolTip(tr("Stop search for duplicates"));

    tb_walk_fail->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    tb_walk_fail->setToolTip(tr("View detail on not readable"));

    lb_walk_fail->setText(tr("Failed to read:"));

    // GB SelectDelete

    gb_select_delete->setTitle(tr("Selection by script"));

    tb_select->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    tb_select->setToolTip(tr("Start selection of duplicates"));

    tb_reload->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    tb_reload->setToolTip(tr("Reload scripts"));

    // GB Action

    gb_action->setTitle(tr("Action"));

    tb_action->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    tb_action->setToolTip(tr("Start the action"));

    tb_action_success->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    tb_action_success->setToolTip(tr("View detail on successful actions"));

    tb_action_fail->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    tb_action_fail->setToolTip(tr("View detail on failed actions"));

    lb_action_success->setText(tr("Successful:"));
    lb_action_success_value->setText("0");

    lb_action_fail->setText(tr("Failed:"));
    lb_action_fail_value->setText("0");

    // GB Progress

    gb_progress->setTitle(tr("Progress"));

    lb_time->setText("");
    lb_overhead->setText("");
    lb_overhead_value->setText("");

    // GB Result

    gb_result->setTitle(tr("Results"));

    // Sorting for the cb_scripts

    auto scripts_sort_proxy = new QSortFilterProxyModel(this);
    scripts_sort_proxy->setSourceModel(cb_scripts->model());
    cb_scripts->setModel(scripts_sort_proxy);

    // Start configuration

    cb_set_config(config_start);

    // Menu connects.

    connect(action_quit, &QAction::triggered, cb_app, &cb_find_duplicates::cb_on_quit);
        /*
    action_about.triggered.connect(self.the_app.on_about)
    action_log.triggered.connect(self.the_app.on_log)

    // Per language.

    action_dutch.triggered.connect(self.the_app.on_language)
    action_english.triggered.connect(self.the_app.on_language)

    // Other connects.

    tb_start_search.clicked.connect(self.the_app.on_start_search)
    tb_stop_search.clicked.connect(self.the_app.on_stop_search)
    tb_walk_fail.clicked.connect(self.the_app.on_walk_fail_detail)

    tb_select.clicked.connect(self.the_app.on_select_by_script)
    tb_reload.clicked.connect(self.the_app.on_update_select_scripts)

    tb_action.clicked.connect(self.the_app.on_action)

    tb_action_success.clicked.connect(self.the_app.on_action_success_detail)
    tb_action_fail.clicked.connect(self.the_app.on_action_fail_detail)
*/
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_main_window::cb_reset_progress_box() 
    {
    lb_done_files->setText("");
    lb_slash->setText("");
    lb_total_files->setText("");

    pb_files->setMinimum(0);
    pb_files->setMaximum(100);
    pb_files->setValue(0);
    pb_files->setVisible(false);

    lb_time->setText("");
    lb_time_value->setText("");

    lb_extra->setText("");
    lb_extra_value->setText("");
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_main_window::cb_set_config(cb_config config)
    {
    if (config == config_start)
        {
        cb_reset_progress_box();
        gb_dir_selector->setEnabled(true);
        tb_start_search->setEnabled(true);
        tb_stop_search->setEnabled(false);
        tb_select->setEnabled(false);
        cb_scripts->setEnabled(true);
        tv_result->setEnabled(false);
        tb_action->setEnabled(false);
        }
    else if (config == config_walking)
        {
        gb_dir_selector->setEnabled(false);
        tb_start_search->setEnabled(false);
        tb_stop_search->setEnabled(true);
        tb_select->setEnabled(false);
        cb_scripts->setEnabled(false);
        tv_result->setEnabled(false);
        tb_action->setEnabled(false);
        }
    else if (config == config_selecting) 
        {
        gb_dir_selector->setEnabled(true);
        tb_start_search->setEnabled(true);
        tb_stop_search->setEnabled(false);
        tb_select->setEnabled(true);
        cb_scripts->setEnabled(true);
        tv_result->setEnabled(true);
        tb_action->setEnabled(true);
        }
    else 
        {
        qFatal(qPrintable(QString("unforeseen config: %1").arg(config)));
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_main_window::closeEvent(QCloseEvent* event)
    {
    qInfo() << __PRETTY_FUNCTION__;
    event->ignore();
    cb_app->cb_on_quit();
    }
    
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_main_window::~cb_main_window()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
