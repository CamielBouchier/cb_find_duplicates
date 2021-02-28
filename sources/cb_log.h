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

#pragma once

#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QObject>

#define  qDLog() qDebug()    << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qILog() qInfo()     << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qWLog() qWarning()  << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"
#define  qELog() qCritical() << "T:" << QThread::currentThreadId() << "f:" << __FUNCTION__<< ":"

class cb_log : public QObject
    {
	Q_OBJECT

    enum log_level {debug, info, warning, error};

    public:
        static void cb_init(const log_level& console_log_level = info,
                            const log_level& file_log_level    = debug);
        static void cb_clean_logdir();

        static QString m_logfile_name;


    private:
	    cb_log(QObject* parent=nullptr);
	    ~cb_log();

	    static void cb_handler(      QtMsgType           type,
                               const QMessageLogContext& context,
                               const QString&            msg);

        static QString   m_logdir;
	    static QFile*    m_file;
        static QMutex    m_mutex;
        static log_level m_console_log_level;
        static log_level m_file_log_level;
    };

//..................................................................................................

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
