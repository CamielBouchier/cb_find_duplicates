//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
//
// (C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)
//
// This file is part of cb_find_duplicates.
// All rights reserved.
// You are granted a non-exclusive and non-transferable license to use this
// software for personal or internal business purposes.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


#include <QCloseEvent>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "cb_abort.h"
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

    // Start configuration

    cb_set_config(config_start);

    // Menu connects.

    auto at = &QAction::triggered;

    connect(action_quit,    at, cb_app, &cb_find_duplicates::cb_on_quit);
    connect(action_about,   at, cb_app, &cb_find_duplicates::cb_on_about);
    connect(action_log,     at, cb_app, &cb_find_duplicates::cb_on_log);
    connect(action_dutch,   at, cb_app, &cb_find_duplicates::cb_on_language);
    connect(action_english, at, cb_app, &cb_find_duplicates::cb_on_language);

    // Other connects.

    auto tbc = &QToolButton::clicked;

    connect(tb_start_search,   tbc, cb_app, &cb_find_duplicates::cb_on_start_search);
    connect(tb_stop_search,    tbc, cb_app, &cb_find_duplicates::cb_on_stop_search);
    connect(tb_walk_fail,      tbc, cb_app, &cb_find_duplicates::cb_on_walk_fail_detail);
    connect(tb_select,         tbc, cb_app, &cb_find_duplicates::cb_on_select_by_script);
    connect(tb_reload,         tbc, cb_app, &cb_find_duplicates::cb_on_update_select_scripts);
    connect(tb_action,         tbc, cb_app, &cb_find_duplicates::cb_on_action);
    connect(tb_action_success, tbc, cb_app, &cb_find_duplicates::cb_on_action_success_detail);
    connect(tb_action_fail,    tbc, cb_app, &cb_find_duplicates::cb_on_action_fail_detail);
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

void cb_main_window::cb_set_config(const cb_config& config)
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
        auto err_msg = QString("unforeseen config: %1").arg(config);
        ABORT(err_msg);
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
