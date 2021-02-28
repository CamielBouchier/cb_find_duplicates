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
                     column_ctime,
                     column_inode,
					 nr_columns};

      	enum action {action_delete, action_link};

      	cb_result_model();
      	~cb_result_model();

      	void cb_do_action(const int action);
      	void cb_log_key_dict() const;
		void cb_reset();
      	void cb_select_by_script(const QString& script_name);
      	void cb_set_result(bool do_recalculate_times_and_inodes = true);


      	uint64_t m_action_fail_count;
      	uint64_t m_action_success_count;
        uint64_t m_nr_file_groups;
      	uint64_t m_overhead_bytes;
        uint64_t m_total_files;

      	QHash<QString, QStringList> m_key_dict;

    protected :

        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder)	override;

        QVariant data(const QModelIndex& the_index, int role = Qt::DisplayRole) const override;

        bool setData(const QModelIndex& the_index,
                     const QVariant& value,
                     int role = Qt::EditRole) override;

        QVariant headerData(int section,
                            Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
    private :

      	void cb_sort();

      	int            m_sort_column;
      	Qt::SortOrder  m_sort_order;

      	QList<QString>                 m_ordered_key_list;
      	QHash<QString, Qt::CheckState> m_check_states;
      	QHash<QString, QDateTime>      m_mtime_dict;
      	QHash<QString, QDateTime>      m_ctime_dict;
      	QHash<QString, int>    		   m_inode_dict;

    friend class cb_less_than;
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
