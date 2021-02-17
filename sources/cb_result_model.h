//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QStandardItemModel>

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class cb_result_model : public QStandardItemModel
    {
    Q_OBJECT
  
    public :

      	enum column {column_basename, 
        	         column_dirname, 
                     column_key, 
                     column_size, 
                     column_mtime, 
                     column_inode,
					 nr_columns};

      	enum action {action_delete, action_link};

      	cb_result_model();
      	~cb_result_model();

      	QHash <QString,QStringList>* cb_get_key_dict()	{ return &m_key_dict; };

      	void cb_set_result       (bool do_recalculate_times = true);
      	void cb_select_by_script (const QString& script_name);
      	void cb_do_action		 (const int action);
		void cb_reset			 ();
	  	
      	void cb_log_key_dict() const;

    protected :
  
        // Overrides from baseclass.
        void          sort   (int column, Qt::SortOrder order = Qt::AscendingOrder)	override;  

        QVariant      data   (const QModelIndex& the_index, 
                                    int          role = Qt::DisplayRole) const		override;

        bool          setData(const QModelIndex& the_index, 
                              const QVariant&    value, 
                                    int          role = Qt::EditRole)					override;

        QVariant      headerData(int             section, 
                                 Qt::Orientation orientation, 
                                 int             role = Qt::DisplayRole) const		override;
    private :

      	void cb_sort();

      	int            m_sort_column;
      	Qt::SortOrder  m_sort_order;

      	int            m_action_success_count;
      	int            m_action_fail_count;   

      	intmax_t       m_overhead_bytes;

      	QHash <QString, Qt::CheckState> m_check_states;
      	QHash <QString, QStringList>    m_key_dict;
      	QList <QString>                 m_ordered_key_list;
      	QHash <QString, QDateTime>      m_mtime_dict;
      	QHash <QString, int>      		m_inode_dict;

      	friend class cb_less_than;
    };
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
