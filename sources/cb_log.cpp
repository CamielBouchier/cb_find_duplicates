//..................................................................................................
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
//..................................................................................................

#include "cb_log.h"

#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

#include "cb_find_duplicates.h"
#include "cb_constants.h"

cb_log::log_level cb_log::m_file_log_level    = debug;
cb_log::log_level cb_log::m_console_log_level = info;

QFile*  cb_log::m_file(nullptr);
QString cb_log::m_logdir("");
QString cb_log::m_logfile_name;
QMutex  cb_log::m_mutex;

//..................................................................................................

cb_log::cb_log(QObject *parent) : QObject(parent)
    {
	qInstallMessageHandler(cb_handler);
    }

//..................................................................................................

cb_log::~cb_log()
    {
	if (m_file)
        {
		m_file->close();
		delete m_file;
	    }
    }

//..................................................................................................

void cb_log::cb_init(const cb_log::log_level& console_log_level,
                     const cb_log::log_level& file_log_level)
    {
    m_console_log_level = console_log_level;
    m_file_log_level    = file_log_level;

    m_logdir = cb_app->m_data_location + "/" + cb_constants::log::logdir_name;

    if (!QDir(m_logdir).exists())
        {
        if (!QDir().mkpath(m_logdir))
            {
            QTextStream(stderr) << "Fatal: could not create '" << m_logdir << "'" << Qt::endl;
            exit(EXIT_FAILURE);
            }
        }

    // logfile creation.

    m_logfile_name =
        m_logdir + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz.log");
    m_file = new QFile(m_logfile_name);
	if (not  m_file->open(QIODevice::WriteOnly))
        {
        QTextStream(stderr) << "Could not open '" << m_logfile_name << "'. Aborting." << Qt::endl;
        exit(EXIT_FAILURE);
        }

    // cb_log instance
	static cb_log log(nullptr);

    QTextStream(stdout) << "Logging in: " << m_logfile_name << Qt::endl;
    }

//..................................................................................................

void cb_log::cb_clean_logdir()
    {
    qInfo() << __PRETTY_FUNCTION__;

    auto& settings = cb_app->m_user_settings;

    int max_entries =
        settings->value("log/max_entries", cb_constants::log::max_entries).toInt();
    int days_to_keep =
        settings->value("log/days_to_keep", cb_constants::log::days_to_keep).toInt();

    settings->setValue("log/max_entries", max_entries);
    settings->setValue("log/days_to_keep", days_to_keep);

    auto now_date = QDateTime::currentDateTime();
    int nr_entries = 0;
    for (const auto& entry_info : QDir(m_logdir).entryInfoList(QDir::Files, QDir::Time))
        {
        nr_entries++;
        auto expiration_date = entry_info.lastModified().addDays(days_to_keep);
        /*
        qDebug() << nr_entries
                 << entry_info.fileName()
                 << entry_info.lastModified().toString()
                 << expiration_date.toString();
        */
        if (now_date > expiration_date or nr_entries > max_entries)
            {
            if (QFile(entry_info.filePath()).remove())
                {
                qDebug() << "Removed:" << entry_info.filePath();
                }
            else
                {
                qDebug() << "Could not remove:" << entry_info.filePath();
                }
            }
        }
    }

//..................................................................................................

void cb_log::cb_handler(QtMsgType msg_type, const QMessageLogContext& context, const QString& msg)
    {
    QMutexLocker locker(&m_mutex);

    QString str_type;
    switch (msg_type)
        {
        case QtDebugMsg:
		    str_type = "D";
		    break;
	    case QtInfoMsg:
		    str_type = "I";
		    break;
	    case QtWarningMsg:
		    str_type = "W";
		    break;
	    case QtCriticalMsg:
		    str_type = "E";
		    break;
	    case QtFatalMsg:
		    str_type = "F";
	    }

    QList<QtMsgType> types = {QtDebugMsg, QtInfoMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg};
    int level = types.indexOf(msg_type);

	QString str_date = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss:zzz");
	QString str_file(context.file);
	QString str_line(QString::number(context.line));
	if (context.line != 0)
        {
		str_file = str_file.mid(str_file.lastIndexOf("\\")+1);
	    }

    #ifdef QT_DEBUG
        QString str_log = QString("%1:%2 %3 %4 %5\n")
                          .arg(str_type)
                          .arg(str_date)
                          .arg(str_file, 32)
                          .arg(str_line, 4)
                          .arg(msg);
    #else
        QString str_log = QString("%1:%2 %3\n")
                          .arg(str_type)
                          .arg(str_date)
                          .arg(msg);
    #endif

    if (level >= m_file_log_level)
        {
	    m_file->write(qPrintable(str_log));
	    m_file->flush();
        }

    if (level >= m_console_log_level)
        {
        QTextStream(stdout) << str_log;
        }

    if (msg_type == QtFatalMsg)
        {
        QTextStream(stderr) << "Aborting." << Qt::endl;
        abort();
        }
    }

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
