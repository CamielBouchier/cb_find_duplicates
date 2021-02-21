//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QDesktopServices>
#include <QMessageBox>
#include <QtCore>

#include "cb_log.h"

#define ABORT(msg)                                                                  \
    {                                                                               \
    qCritical() << (msg) << Qt::endl;                                               \
    QMessageBox::critical(nullptr, QObject::tr("Aborting"), (msg));                 \
    QDesktopServices::openUrl(QUrl(cb_log::m_logfile_name, QUrl::TolerantMode));    \
    abort();                                                                        \
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
