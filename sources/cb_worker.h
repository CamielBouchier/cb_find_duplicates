//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QtCore>

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class cb_worker : public QObject
    {
    Q_OBJECT
  
    public :
  
      	cb_worker(QHash <QString,QStringList>* key_dict);
      	~cb_worker();
		

    public slots :

      	void cb_walk(const QStringList& dirs);
      	void cb_stop();

    signals :

      	void cb_signal_done(bool ok);

    private :

		bool cb_do_gui_communication()	const;
      	void cb_log_key_dict() 			const;

		QString 	m_msg_progress;
		intmax_t	m_walk_fail_count;
		intmax_t	m_done_files;
		intmax_t	m_total_files;

      	QHash <QString,QStringList>*  m_key_dict;

      	volatile bool m_walk;

    };
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
