//..................................................................................................
//
// $BeginLicense$
// $EndLicense$
//
//..................................................................................................

#pragma once
 
#include <QDebug>
#include <QObject>
#include <QThread>
 
#define  qDLog() qDebug()    << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qILog() qInfo()     << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qWLog() qWarning()  << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qELog() qCritical() << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
 
class QFile;
 
class cb_log : public QObject
    {
	Q_OBJECT

    enum log_level {L_DEBUG, L_INFO, L_WARNING, L_ERROR};

    public:
        static void init(log_level console_log_level = L_INFO,
                         log_level file_log_level    = L_DEBUG);

    private:
	    cb_log(QObject* parent=nullptr);
	    ~cb_log();

	    static void cb_handler(      QtMsgType           type, 
                               const QMessageLogContext& context, 
                               const QString&            msg);

	    static QFile*    m_file;
        static log_level m_console_log_level;
        static log_level m_file_log_level;
    };
 
//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
