/********************************************************************************
** Form generated from reading UI file 'cb_main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CB_MAIN_WINDOW_H
#define UI_CB_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cb_main_window
{
public:
    QAction *action_quit;
    QAction *action_about;
    QAction *action_dutch;
    QAction *action_english;
    QAction *action_log;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_10;
    QSplitter *main_splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *gb_dir_selector;
    QVBoxLayout *verticalLayout;
    QTreeView *tv_dir_selector;
    QGroupBox *gb_start_stop;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout_3;
    QToolButton *tb_start_search;
    QToolButton *tb_stop_search;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lb_walk_fail;
    QLabel *lb_walk_fail_value;
    QToolButton *tb_walk_fail;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *gb_select_delete;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *tb_select;
    QComboBox *cb_scripts;
    QToolButton *tb_reload;
    QSpacerItem *horizontalSpacer;
    QGroupBox *gb_action;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *tb_action;
    QComboBox *cb_actions;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_2;
    QLabel *lb_action_success;
    QLabel *lb_action_success_value;
    QToolButton *tb_action_success;
    QSpacerItem *horizontalSpacer_4;
    QLabel *lb_action_fail;
    QLabel *lb_action_fail_value;
    QToolButton *tb_action_fail;
    QGroupBox *gb_result;
    QVBoxLayout *verticalLayout_2;
    QTableView *tv_result;
    QGroupBox *gb_progress;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lb_done_files;
    QLabel *lb_slash;
    QLabel *lb_total_files;
    QProgressBar *pb_files;
    QLabel *lb_time;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lb_time_value;
    QSpacerItem *horizontalSpacer_5;
    QLabel *lb_overhead;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lb_overhead_value;
    QSpacerItem *horizontalSpacer_7;
    QLabel *lb_extra;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lb_extra_value;
    QSpacerItem *horizontalSpacer_8;
    QMenuBar *menubar;
    QMenu *menuDatabase;
    QMenu *menuAbout;
    QMenu *menuLanguage;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *cb_main_window)
    {
        if (cb_main_window->objectName().isEmpty())
            cb_main_window->setObjectName(QString::fromUtf8("cb_main_window"));
        cb_main_window->resize(1341, 862);
        cb_main_window->setWindowTitle(QString::fromUtf8("main_window"));
        action_quit = new QAction(cb_main_window);
        action_quit->setObjectName(QString::fromUtf8("action_quit"));
        action_about = new QAction(cb_main_window);
        action_about->setObjectName(QString::fromUtf8("action_about"));
        action_dutch = new QAction(cb_main_window);
        action_dutch->setObjectName(QString::fromUtf8("action_dutch"));
        action_english = new QAction(cb_main_window);
        action_english->setObjectName(QString::fromUtf8("action_english"));
        action_log = new QAction(cb_main_window);
        action_log->setObjectName(QString::fromUtf8("action_log"));
        centralwidget = new QWidget(cb_main_window);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_10 = new QVBoxLayout(centralwidget);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        main_splitter = new QSplitter(centralwidget);
        main_splitter->setObjectName(QString::fromUtf8("main_splitter"));
        main_splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(main_splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        gb_dir_selector = new QGroupBox(layoutWidget);
        gb_dir_selector->setObjectName(QString::fromUtf8("gb_dir_selector"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(gb_dir_selector->sizePolicy().hasHeightForWidth());
        gb_dir_selector->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(gb_dir_selector);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tv_dir_selector = new QTreeView(gb_dir_selector);
        tv_dir_selector->setObjectName(QString::fromUtf8("tv_dir_selector"));

        verticalLayout->addWidget(tv_dir_selector);


        verticalLayout_5->addWidget(gb_dir_selector);

        gb_start_stop = new QGroupBox(layoutWidget);
        gb_start_stop->setObjectName(QString::fromUtf8("gb_start_stop"));
        gb_start_stop->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_4 = new QVBoxLayout(gb_start_stop);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tb_start_search = new QToolButton(gb_start_stop);
        tb_start_search->setObjectName(QString::fromUtf8("tb_start_search"));

        gridLayout_3->addWidget(tb_start_search, 0, 2, 1, 1);

        tb_stop_search = new QToolButton(gb_start_stop);
        tb_stop_search->setObjectName(QString::fromUtf8("tb_stop_search"));

        gridLayout_3->addWidget(tb_stop_search, 0, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 4, 1, 1);

        lb_walk_fail = new QLabel(gb_start_stop);
        lb_walk_fail->setObjectName(QString::fromUtf8("lb_walk_fail"));
        lb_walk_fail->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(lb_walk_fail, 1, 0, 1, 1);

        lb_walk_fail_value = new QLabel(gb_start_stop);
        lb_walk_fail_value->setObjectName(QString::fromUtf8("lb_walk_fail_value"));
        lb_walk_fail_value->setText(QString::fromUtf8("0"));

        gridLayout_3->addWidget(lb_walk_fail_value, 1, 1, 1, 1);

        tb_walk_fail = new QToolButton(gb_start_stop);
        tb_walk_fail->setObjectName(QString::fromUtf8("tb_walk_fail"));

        gridLayout_3->addWidget(tb_walk_fail, 1, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 1, 4, 1, 1);


        verticalLayout_4->addLayout(gridLayout_3);


        verticalLayout_5->addWidget(gb_start_stop);

        gb_select_delete = new QGroupBox(layoutWidget);
        gb_select_delete->setObjectName(QString::fromUtf8("gb_select_delete"));
        verticalLayout_3 = new QVBoxLayout(gb_select_delete);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tb_select = new QToolButton(gb_select_delete);
        tb_select->setObjectName(QString::fromUtf8("tb_select"));

        horizontalLayout_2->addWidget(tb_select);

        cb_scripts = new QComboBox(gb_select_delete);
        cb_scripts->setObjectName(QString::fromUtf8("cb_scripts"));
        cb_scripts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_2->addWidget(cb_scripts);

        tb_reload = new QToolButton(gb_select_delete);
        tb_reload->setObjectName(QString::fromUtf8("tb_reload"));

        horizontalLayout_2->addWidget(tb_reload);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(gb_select_delete);

        gb_action = new QGroupBox(layoutWidget);
        gb_action->setObjectName(QString::fromUtf8("gb_action"));
        verticalLayout_6 = new QVBoxLayout(gb_action);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tb_action = new QToolButton(gb_action);
        tb_action->setObjectName(QString::fromUtf8("tb_action"));

        horizontalLayout_3->addWidget(tb_action);

        cb_actions = new QComboBox(gb_action);
        cb_actions->setObjectName(QString::fromUtf8("cb_actions"));
        cb_actions->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_3->addWidget(cb_actions);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_6->addLayout(horizontalLayout_3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lb_action_success = new QLabel(gb_action);
        lb_action_success->setObjectName(QString::fromUtf8("lb_action_success"));
        lb_action_success->setText(QString::fromUtf8("lb_success"));

        gridLayout_2->addWidget(lb_action_success, 0, 0, 1, 1);

        lb_action_success_value = new QLabel(gb_action);
        lb_action_success_value->setObjectName(QString::fromUtf8("lb_action_success_value"));
        lb_action_success_value->setText(QString::fromUtf8("lb_success_value"));

        gridLayout_2->addWidget(lb_action_success_value, 0, 1, 1, 1);

        tb_action_success = new QToolButton(gb_action);
        tb_action_success->setObjectName(QString::fromUtf8("tb_action_success"));

        gridLayout_2->addWidget(tb_action_success, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 3, 2, 1);

        lb_action_fail = new QLabel(gb_action);
        lb_action_fail->setObjectName(QString::fromUtf8("lb_action_fail"));
        lb_action_fail->setText(QString::fromUtf8("lb_fail"));

        gridLayout_2->addWidget(lb_action_fail, 1, 0, 1, 1);

        lb_action_fail_value = new QLabel(gb_action);
        lb_action_fail_value->setObjectName(QString::fromUtf8("lb_action_fail_value"));
        lb_action_fail_value->setText(QString::fromUtf8("lb_fail_value"));

        gridLayout_2->addWidget(lb_action_fail_value, 1, 1, 1, 1);

        tb_action_fail = new QToolButton(gb_action);
        tb_action_fail->setObjectName(QString::fromUtf8("tb_action_fail"));

        gridLayout_2->addWidget(tb_action_fail, 1, 2, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);


        verticalLayout_5->addWidget(gb_action);

        main_splitter->addWidget(layoutWidget);
        gb_dir_selector->raise();
        gb_select_delete->raise();
        gb_action->raise();
        gb_start_stop->raise();
        gb_result = new QGroupBox(main_splitter);
        gb_result->setObjectName(QString::fromUtf8("gb_result"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gb_result->sizePolicy().hasHeightForWidth());
        gb_result->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(gb_result);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tv_result = new QTableView(gb_result);
        tv_result->setObjectName(QString::fromUtf8("tv_result"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tv_result->sizePolicy().hasHeightForWidth());
        tv_result->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(tv_result);

        gb_progress = new QGroupBox(gb_result);
        gb_progress->setObjectName(QString::fromUtf8("gb_progress"));
        gridLayout = new QGridLayout(gb_progress);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lb_done_files = new QLabel(gb_progress);
        lb_done_files->setObjectName(QString::fromUtf8("lb_done_files"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lb_done_files->sizePolicy().hasHeightForWidth());
        lb_done_files->setSizePolicy(sizePolicy3);
        lb_done_files->setText(QString::fromUtf8("LABEL_DONEFILES"));
        lb_done_files->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(lb_done_files);

        lb_slash = new QLabel(gb_progress);
        lb_slash->setObjectName(QString::fromUtf8("lb_slash"));

        horizontalLayout->addWidget(lb_slash);

        lb_total_files = new QLabel(gb_progress);
        lb_total_files->setObjectName(QString::fromUtf8("lb_total_files"));
        sizePolicy3.setHeightForWidth(lb_total_files->sizePolicy().hasHeightForWidth());
        lb_total_files->setSizePolicy(sizePolicy3);
        lb_total_files->setText(QString::fromUtf8("LABEL_TOTALFILES"));

        horizontalLayout->addWidget(lb_total_files);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        pb_files = new QProgressBar(gb_progress);
        pb_files->setObjectName(QString::fromUtf8("pb_files"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pb_files->sizePolicy().hasHeightForWidth());
        pb_files->setSizePolicy(sizePolicy4);
        pb_files->setValue(24);

        gridLayout->addWidget(pb_files, 0, 1, 1, 1);

        lb_time = new QLabel(gb_progress);
        lb_time->setObjectName(QString::fromUtf8("lb_time"));
        sizePolicy3.setHeightForWidth(lb_time->sizePolicy().hasHeightForWidth());
        lb_time->setSizePolicy(sizePolicy3);
        lb_time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lb_time, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lb_time_value = new QLabel(gb_progress);
        lb_time_value->setObjectName(QString::fromUtf8("lb_time_value"));
        lb_time_value->setText(QString::fromUtf8("lb_time_value"));

        horizontalLayout_5->addWidget(lb_time_value);

        horizontalSpacer_5 = new QSpacerItem(390, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        gridLayout->addLayout(horizontalLayout_5, 1, 1, 1, 1);

        lb_overhead = new QLabel(gb_progress);
        lb_overhead->setObjectName(QString::fromUtf8("lb_overhead"));
        lb_overhead->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lb_overhead, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lb_overhead_value = new QLabel(gb_progress);
        lb_overhead_value->setObjectName(QString::fromUtf8("lb_overhead_value"));

        horizontalLayout_4->addWidget(lb_overhead_value);

        horizontalSpacer_7 = new QSpacerItem(390, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);


        gridLayout->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        lb_extra = new QLabel(gb_progress);
        lb_extra->setObjectName(QString::fromUtf8("lb_extra"));
        lb_extra->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lb_extra, 3, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        lb_extra_value = new QLabel(gb_progress);
        lb_extra_value->setObjectName(QString::fromUtf8("lb_extra_value"));

        horizontalLayout_6->addWidget(lb_extra_value);

        horizontalSpacer_8 = new QSpacerItem(390, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);


        gridLayout->addLayout(horizontalLayout_6, 3, 1, 1, 1);


        verticalLayout_2->addWidget(gb_progress);

        main_splitter->addWidget(gb_result);

        verticalLayout_10->addWidget(main_splitter);

        cb_main_window->setCentralWidget(centralwidget);
        menubar = new QMenuBar(cb_main_window);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1341, 21));
        menuDatabase = new QMenu(menubar);
        menuDatabase->setObjectName(QString::fromUtf8("menuDatabase"));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuLanguage = new QMenu(menubar);
        menuLanguage->setObjectName(QString::fromUtf8("menuLanguage"));
        cb_main_window->setMenuBar(menubar);
        statusbar = new QStatusBar(cb_main_window);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        cb_main_window->setStatusBar(statusbar);

        menubar->addAction(menuDatabase->menuAction());
        menubar->addAction(menuLanguage->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuDatabase->addAction(action_quit);
        menuAbout->addAction(action_about);
        menuAbout->addAction(action_log);
        menuLanguage->addAction(action_dutch);
        menuLanguage->addAction(action_english);

        retranslateUi(cb_main_window);

        QMetaObject::connectSlotsByName(cb_main_window);
    } // setupUi

    void retranslateUi(QMainWindow *cb_main_window)
    {
        action_quit->setText(QCoreApplication::translate("cb_main_window", "Quit", nullptr));
        action_about->setText(QCoreApplication::translate("cb_main_window", "About", nullptr));
        action_dutch->setText(QCoreApplication::translate("cb_main_window", "Dutch", nullptr));
        action_english->setText(QCoreApplication::translate("cb_main_window", "English", nullptr));
        action_log->setText(QCoreApplication::translate("cb_main_window", "Log", nullptr));
        gb_dir_selector->setTitle(QCoreApplication::translate("cb_main_window", "gb_dir_selector", nullptr));
        gb_start_stop->setTitle(QCoreApplication::translate("cb_main_window", "gb_start_stop", nullptr));
        tb_start_search->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        tb_stop_search->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        lb_walk_fail->setText(QCoreApplication::translate("cb_main_window", "lb_walk_fail", nullptr));
        tb_walk_fail->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        gb_select_delete->setTitle(QCoreApplication::translate("cb_main_window", "gb_select_delete", nullptr));
#if QT_CONFIG(tooltip)
        tb_select->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        tb_select->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        tb_reload->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        gb_action->setTitle(QCoreApplication::translate("cb_main_window", "gb_action", nullptr));
        tb_action->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        tb_action_success->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        tb_action_fail->setText(QCoreApplication::translate("cb_main_window", "...", nullptr));
        gb_result->setTitle(QCoreApplication::translate("cb_main_window", "gb_result", nullptr));
        gb_progress->setTitle(QCoreApplication::translate("cb_main_window", "gb_progress", nullptr));
#if QT_CONFIG(tooltip)
        lb_done_files->setToolTip(QCoreApplication::translate("cb_main_window", "Handled number of files", nullptr));
#endif // QT_CONFIG(tooltip)
        lb_slash->setText(QCoreApplication::translate("cb_main_window", "/", nullptr));
#if QT_CONFIG(tooltip)
        lb_total_files->setToolTip(QCoreApplication::translate("cb_main_window", "Number of files to be handled", nullptr));
#endif // QT_CONFIG(tooltip)
        lb_time->setText(QCoreApplication::translate("cb_main_window", "lb_time", nullptr));
        lb_overhead->setText(QCoreApplication::translate("cb_main_window", "lb_overhead", nullptr));
        lb_overhead_value->setText(QCoreApplication::translate("cb_main_window", "lb_overhead_value", nullptr));
        lb_extra->setText(QCoreApplication::translate("cb_main_window", "lb_extra", nullptr));
        lb_extra_value->setText(QCoreApplication::translate("cb_main_window", "lb_extra_value", nullptr));
        menuDatabase->setTitle(QCoreApplication::translate("cb_main_window", "File", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("cb_main_window", "About", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("cb_main_window", "Language", nullptr));
        (void)cb_main_window;
    } // retranslateUi

};

namespace Ui {
    class cb_main_window: public Ui_cb_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CB_MAIN_WINDOW_H
