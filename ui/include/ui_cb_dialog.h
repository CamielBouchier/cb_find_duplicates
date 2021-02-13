/********************************************************************************
** Form generated from reading UI file 'cb_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CB_DIALOG_H
#define UI_CB_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_cb_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *tb_text;
    QHBoxLayout *horizontalLayout;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *cb_dialog)
    {
        if (cb_dialog->objectName().isEmpty())
            cb_dialog->setObjectName(QString::fromUtf8("cb_dialog"));
        cb_dialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(cb_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, -1);
        tb_text = new QTextBrowser(cb_dialog);
        tb_text->setObjectName(QString::fromUtf8("tb_text"));

        verticalLayout->addWidget(tb_text);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonBox = new QDialogButtonBox(cb_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);

        horizontalSpacer = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(cb_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), cb_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), cb_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(cb_dialog);
    } // setupUi

    void retranslateUi(QDialog *cb_dialog)
    {
        cb_dialog->setWindowTitle(QCoreApplication::translate("cb_dialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cb_dialog: public Ui_cb_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CB_DIALOG_H
