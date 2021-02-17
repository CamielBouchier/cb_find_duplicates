//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "cb_abort.h"
#include "cb_find_duplicates.h"
#include "cb_qfile.h"
#include "cb_support.h"
#include "cb_worker.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_worker::cb_worker(QHash <QString,QStringList>* key_dict)
  	{
  	qInfo() << __PRETTY_FUNCTION__;

  	m_key_dict = key_dict;
  	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_worker::cb_walk(const QStringList& dirs)
  	{
  	qInfo() << __PRETTY_FUNCTION__;

  	m_msg_progress = tr("Searching for duplicates");

    cb_qfile logfile_fail(cb_app->m_walk_fail_filename,
                          QIODevice::WriteOnly | QIODevice::Text,
                          __FILE__,__LINE__);
  
	m_walk_fail_count = 0;

  	m_walk = true;
  	m_key_dict->clear();

  	// Progress meter.
	m_done_files = 0;
  	m_total_files = 0;

  	m_msg_progress = tr("Determining number of files.");

  	for (auto&& dir : dirs)
    	{
    	if (not cb_do_gui_communication()) break;
    	QDirIterator dir_iterator(dir, 
                                  QDir::Files | QDir::NoSymLinks | QDir::Hidden,
                                  QDirIterator::Subdirectories);
    	while (dir_iterator.hasNext())
      		{
    		if (not cb_do_gui_communication()) break;
      		m_total_files++;
      		dir_iterator.next();
      		}
    	}

  	m_msg_progress = tr("Start processing dirs.");

  	for (auto&& dir : dirs)
    	{
    	if (not cb_do_gui_communication()) break;
    	QDirIterator dir_iterator(dir, 
                                  QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::Hidden,
                                  QDirIterator::Subdirectories);
    	while (dir_iterator.hasNext())
      		{
    		if (not cb_do_gui_communication()) break;
      		auto next = dir_iterator.next();
      		// Additional check/report on unreadable directories.
      		if (QFileInfo(next).isDir())
        		{
        		if (not QFileInfo(next).isReadable())
          			{
          			QTextStream(&logfile_fail) << tr("Not readable directory:")
							                   << next
											   << Qt::endl;
          			m_walk_fail_count++;
          			}
        		continue;
        		}
      		auto file = QFileInfo(next).canonicalFilePath();
      		if (QFileInfo(file).isReadable())
        		{
        		auto key = QString::number(QFileInfo(file).size());
        		if (m_key_dict->contains(key))
          			{
          			(*m_key_dict)[key].append(file);
          			}
        		else
          			{
          			(*m_key_dict)[key] = QStringList(file);
          			}
        		}
      		else
        		{
        		QTextStream(&logfile_fail) << tr("Not readable:")
						                   << file 
										   << Qt::endl;
        		m_walk_fail_count++;
        		}
       		m_done_files ++;
      		}
    	}


  	// At this point we have a dictionary key(unique on Size)=>ListOfFiles
  	// Now doing partial (phase=0) and full (phase=1) MD5 calculations to break the tie

  	for (auto phase = 0; phase < 2; phase++) 
    	{
    	if (not cb_do_gui_communication()) break;
		qInfo() << tr("Start of") 
	            << (phase == 0 ? "partial":"full")
				<< "md5 sum calculation.";
      	// log_key_dict();

     	m_done_files  = 0;
    	m_total_files = 0;

		if (phase == 0)
			{
			m_msg_progress = tr("Calculating partial MD5 for potential duplicates."); 
			}
		else
			{
            m_msg_progress = tr("Calculating full MD5 for potential duplicates") ; 
			}
  
    	for (auto&& key : m_key_dict->keys())
      		{
    		if (not cb_do_gui_communication()) break;
      		auto& files = (*m_key_dict)[key];
      		auto nr_files = files.size();
      		// On the fly remove all entries that don't have at least 2 files.
      		if (nr_files < 2)
        		{
        		m_key_dict->remove(key);
        		}
      		else
        		{
        		m_total_files += nr_files;
        		}
      		}
  
    	// Start the MD5 calculation
    	auto old_keys = m_key_dict->keys();
    	#ifdef _OPENMP
      		#pragma omp parallel for default(shared) schedule(dynamic)
    	#endif
    	for (int idx = 0; idx < old_keys.size(); idx++)
      		{
    		if (not cb_do_gui_communication()) continue;
      		QString     key;
      		QStringList files;
      		int         nr_files;
      		#ifdef _OPENMP
        		#pragma omp critical
        		{
        		key     = old_keys.at(idx);
        		files   = (*m_key_dict)[key];
        		nr_files = files.size();
        		if (nr_files < 2) 
          			{
          			ABORT(tr("Internal error. nr_files < 2 for key '%1': %2")
						  .arg(key).arg(nr_files));
          			}
        		}
      		#endif

      		// No point in calculating tie if it are all the same files.
      		if (cb_is_same_file(files)) 
        		{
        		continue;
        		}

      		for (auto&& file : files)
        		{
          		auto md5_sum = cb_md5_sum(file, /*OneChunkOnly=*/ (phase == 0));
        		auto new_key = key + "_" + md5_sum;
        		#ifdef _OPENMP
          			#pragma omp critical
         	 		{
          			m_key_dict->remove(key);
          			if (m_key_dict->contains(new_key))
            			{
            			(*m_key_dict)[new_key].append(file);
            			}
          			else
            			{
            			(*m_key_dict)[new_key] = QStringList(file);
            			}
           			m_done_files++;
          			}
        		#endif
        		}
      		}
  		}
  
  	// Remove keys that don't have at least 2 files.
  	m_msg_progress = tr("Cleaning unique files.");
  	for (auto&& key : m_key_dict->keys())
    	{
    	if (not cb_do_gui_communication()) break;
    	auto& files   = (*m_key_dict)[key];
    	auto  nr_files = files.size();
    	if (nr_files < 2)
      		{
      		m_key_dict->remove(key);
      		}
    	}

  	// Done
  	if (not m_walk)
    	{
    	m_key_dict->clear();
    	}
  	m_msg_progress = m_walk ? tr("Done") : tr("Aborted");
  
	emit cb_signal_done(m_walk);
  	m_walk = false;
  	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_worker::cb_do_gui_communication() const
	{
	return m_walk;
	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_worker::cb_stop()
  	{
  	qInfo() << __PRETTY_FUNCTION__;

  	m_walk = false;
  	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_worker::cb_log_key_dict() const
  	{
  	qInfo() << __PRETTY_FUNCTION__;

  	for (auto&& key : m_key_dict->keys())
    	{
    	auto& files    = (*m_key_dict)[key];
    	auto  nr_files = files.size();
		qInfo() << QString("key_dict[%1](%2) : %3")
			       .arg(key,80).arg(nr_files,3).arg(cb_represent(files));
    }
  }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include <iostream>

cb_worker::~cb_worker() 
  	{
  	qInfo() << __PRETTY_FUNCTION__;
	std::cout << __PRETTY_FUNCTION__;
  	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
