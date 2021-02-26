//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

#include "cb_abort.h"
#include "cb_find_duplicates.h"
#include "cb_result_model.h"
#include "cb_support.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_result_model::cb_result_model() : QStandardItemModel(nullptr)
    {
    qInfo() << __PRETTY_FUNCTION__;

    setColumnCount(nr_columns);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_reset()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_key_dict.clear();
    m_ordered_key_list.clear();
    m_mtime_dict.clear();
    m_inode_dict.clear();
    m_check_states.clear();
    m_overhead_bytes = 0;
    removeRows(0, rowCount());
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QVariant cb_result_model::data(const QModelIndex& the_index, int role) const
    {
    if (role == Qt::BackgroundRole)
      	{
      	auto color_index = item(the_index.row(), column_basename)->data(Qt::UserRole+3).toInt();
      	if (color_index == 0)
        	{
        	return QStandardItemModel::data(the_index, role);
        	}
      	return QBrush(QColor(0xFF,0xE0,0xCC));
      	}
    else if ((role == Qt::CheckStateRole) and (the_index.column() == 0))
      	{
      	auto file = item(the_index.row(), column_basename)->data(Qt::UserRole+2).toString();
      	if (m_check_states.contains(file))
        	{
        	return m_check_states.value(file);
        	}
      	else 
        	{
        	return Qt::Unchecked;
        	}
      	}
    return QStandardItemModel::data(the_index,role);
	}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_result_model::setData(const QModelIndex& the_index, const QVariant& value, int role)
    {
    if (not ((role == Qt::CheckStateRole) and (the_index.column() == 0)))
        {
        return QStandardItemModel::setData(the_index, value, role);
        }

    auto key  = item(the_index.row(), column_basename)->data(Qt::UserRole+1).toString();
    auto file = item(the_index.row(), column_basename)->data(Qt::UserRole+2).toString();

    auto check_value = static_cast <Qt::CheckState> (value.toInt());
    m_check_states[file] = check_value;

    // Ensure at least one file for key is "unchecked".
    if (check_value == Qt::Checked)
        {
        bool all_checked = true;
        for (const auto& f : m_key_dict[key])
            {
            if (not m_check_states.contains(f) or (m_check_states.value(f) == Qt::Unchecked))
                {
                all_checked = false;
                break;
                }
            }
        if (all_checked)
            {
            m_check_states[file] = Qt::Unchecked;
            }
        }
    emit dataChanged(the_index, the_index);     // Signal the change.
    return true;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::sort(int column, Qt::SortOrder order)
    {
    m_sort_column = column;
    m_sort_order  = order;
    cb_set_result(false);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QVariant cb_result_model::headerData(int section, Qt::Orientation orientation, int role) const
    {
    if (role != Qt::DisplayRole or orientation != Qt::Orientation::Horizontal)
        {
        return QStandardItemModel::headerData(section, orientation, role);
        }
    switch (section) 
        {
        case column_basename : return tr("Filename");
        case column_dirname  : return tr("Directory");
        case column_key      : return tr("Key");
        case column_size     : return tr("Size");
        case column_mtime    : return tr("Modification time");
        case column_inode    : return tr("Inode");
        default : 
            auto err_msg = tr("Unforeseen column: %1").arg(section);
            ABORT(err_msg);
        }
    return QVariant();
    } 

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_set_result(bool do_recalculate_times_and_inodes)
    {
    qInfo() << __PRETTY_FUNCTION__ << do_recalculate_times_and_inodes;

    // m_mtime_dict, m_inode_dict are basically caches that
    // need to be calculate first time, but not after a cb_set_result due to sorting or hiding.
    if (do_recalculate_times_and_inodes) 
        {
        m_mtime_dict.clear();
        m_inode_dict.clear();
        for (const auto& key : m_key_dict.keys()) 
            {
            for (const auto& file: m_key_dict[key])
                {
                m_mtime_dict[file] = QFileInfo(file).lastModified();
                m_inode_dict[file] = cb_get_fake_inode(file);
                }
            }
        }

    // Delete all entries in the ResultTable, sort the data and add them.
    removeRows(0, rowCount());
    cb_sort();

    int color_index = 0;

    m_overhead_bytes = 0;
    m_total_files    = 0;
    m_nr_file_groups = 0;

    // Start adding them again.
    for (const auto& key: m_ordered_key_list) 
        {
        m_nr_file_groups++;

        if (not m_key_dict.contains(key)) // m_ordered_key_list must have same keys as m_key_dict.
            {
            auto err_msg = tr("Internal error. Expected key '%1' not in m_key_dict.").arg(key);
            ABORT(err_msg);
            }

        auto size = cb_size_from_key(key);
        m_overhead_bytes -= size;   // Compensating addition of first in group.

        for (const auto& file: m_key_dict[key])
            {
            m_total_files++;
            m_overhead_bytes += size;

            auto basename_item = new QStandardItem(QFileInfo(file).fileName());
            auto dirname_item  = new QStandardItem(QFileInfo(file).path());
            auto key_item      = new QStandardItem(key);
            auto size_item     = new QStandardItem(cb_sizestring_from_key(key));
            auto mtime_item    = new QStandardItem(m_mtime_dict[file].toString());
            auto inode_item    = new QStandardItem(QString::number(m_inode_dict[file]));

            basename_item->setData(key,         Qt::UserRole+1);
            basename_item->setData(file,        Qt::UserRole+2);
            basename_item->setData(color_index, Qt::UserRole+3);

            QList <QStandardItem*> row;
            for (int column = 0; column < nr_columns; column++) 
                {
                switch (column) 
                    {
                    case column_basename : row.append(basename_item); break;
                    case column_dirname  : row.append(dirname_item);  break;
                    case column_key      : row.append(key_item);      break;
                    case column_size     : row.append(size_item);     break;
                    case column_mtime    : row.append(mtime_item);    break;
                    case column_inode    : row.append(inode_item);    break;
                    default : 
                        auto err_msg = tr("Unforeseen column (%1)").arg(column);
                        ABORT(err_msg);
                    }
                } 
            row[0]->setCheckable(true);
            appendRow(row);
            }
        color_index ^= 1;
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Sorting , first files within each key ('Inner') , and then keys.
// Supported by "Functors" to accomodate for qSort's cb_less_than getting access to members.

class cb_less_than 
    {
    public :
        cb_less_than(cb_result_model* ptr, bool inner_sort) 
            {
            m_model = ptr; 
            m_inner_sort = inner_sort;
            };
        bool operator()(const QString& key1, const QString& key2)  const;
    private :
        cb_result_model* m_model;
        bool             m_inner_sort;
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_less_than::operator()(const QString& key1, const QString& key2) const
    {
    bool rv;
    const auto& sort_column = m_model->m_sort_column;

    if (m_inner_sort)
        {
        if (sort_column == cb_result_model::column_basename)
            {
            rv = ( QFileInfo(key1).fileName() < QFileInfo(key2).fileName() ); 
            }
        else if (sort_column == cb_result_model::column_mtime)
            {
            rv = ( m_model->m_mtime_dict[key1] < m_model->m_mtime_dict[key2] ); 
            }
        else if (sort_column == cb_result_model::column_inode)
            {
            rv = ( m_model->m_inode_dict[key1] < m_model->m_inode_dict[key2] ); 
            }
        else
            {
            rv = ( QFileInfo(key1).path() < QFileInfo(key2).path() ); 
            }
        }
    else
        {
        if (sort_column == cb_result_model::column_basename)
            {
            QString file1 = m_model->m_key_dict[key1].at(0);
            QString file2 = m_model->m_key_dict[key2].at(0);
            rv = ( QFileInfo(file1).fileName() < QFileInfo(file2).fileName() ); 
            }
        else if (sort_column == cb_result_model::column_dirname)
            {
            QString file1 = m_model->m_key_dict[key1].at(0);
            QString file2 = m_model->m_key_dict[key2].at(0);
            rv = ( QFileInfo(file1).path() < QFileInfo(file2).path() ); 
            }
        else if (sort_column == cb_result_model::column_key)
            {
            rv = ( key1 < key2 ); 
            }
        else if (sort_column == cb_result_model::column_size)
            {
            rv = ( cb_size_from_key(key1) < cb_size_from_key(key2) ); 
            }
        else if (sort_column == cb_result_model::column_mtime)
            {
            QString file1 = m_model->m_key_dict[key1].at(0);
            QString file2 = m_model->m_key_dict[key2].at(0);
            rv = ( m_model->m_mtime_dict[file1] < m_model->m_mtime_dict[file2] ); 
            }
        else if (sort_column == cb_result_model::column_inode)
            {
            QString file1 = m_model->m_key_dict[key1].at(0);
            QString file2 = m_model->m_key_dict[key2].at(0);
            rv = ( m_model->m_inode_dict[file1] < m_model->m_inode_dict[file2] ); 
            }
        else 
            {
            auto err_msg = QObject::tr("Unforeseen column (%1)").arg(m_model->m_sort_column);
            ABORT(err_msg);
            }
        }

    if (m_model->m_sort_order == Qt::SortOrder::DescendingOrder) 
        {
        rv = not rv;
        }

    return rv;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_sort()
    {
    m_ordered_key_list.clear();
    for (const auto& key : m_key_dict.keys())
        {
        m_ordered_key_list.append(key);
        auto& files = m_key_dict[key];
        std::sort(files.begin(), files.end(), cb_less_than(this, true));
        }
    std::sort(m_ordered_key_list.begin(), m_ordered_key_list.end(), cb_less_than(this, false));
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_select_by_script(const QString &script_name)
    {
    qInfo() << __PRETTY_FUNCTION__;

    for (const auto& key : m_key_dict.keys())
        {
        const auto& files = m_key_dict[key];
        QList <bool> selected;
        QList <uint> times;
        for (int i = 0; i < files.size(); i++)
            {
            selected.append(false);
            times.append(m_mtime_dict[files.at(i)].toTime_t());
            }

        QString message;
        bool ok;
        cb_app->m_lua_selector->cb_call_script(script_name, 
                                               files, 
                                               times, 
                                               cb_size_from_key(key), 
                                               selected, 
                                               ok, 
                                               message);
        if (not ok)
            {
            QMessageBox::warning(cb_app->m_main_window.get(),
                                 tr("Error in script"),
                                 tr("An error was found.\nScript '%1'.\nError : %2").
                                    arg(script_name).arg(message));
            return;
            }

        for (int i=0; i<files.size(); i++)
            {
            m_check_states[files.at(i)] = selected.at(i) ? Qt::Checked : Qt::Unchecked;
            }
        }
    cb_set_result(false);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_do_action(const int action)
    {
    qInfo() << __PRETTY_FUNCTION__;

    QFile logfile_success(cb_app->m_action_success_filename);
    if (not logfile_success.open(QIODevice::WriteOnly | QIODevice::Text))
        {
        auto err_msg = tr("Could not open: '%1'.").arg(cb_app->m_action_success_filename);
        ABORT(err_msg);
        }
                                     
    QFile logfile_fail(cb_app->m_action_fail_filename);
    if (not logfile_fail.open(QIODevice::WriteOnly | QIODevice::Text))
        {
        auto err_msg = tr("Could not open: '%1'.").arg(cb_app->m_action_fail_filename);
        ABORT(err_msg);
        }
                                     
    m_action_success_count = 0;
    m_action_fail_count    = 0;

    for (const auto &key : m_key_dict.keys())
        {
        const auto &files = m_key_dict[key];

        // Find first unchecked for the case of Link.
        QString unchecked_file;
        if (action == action_link)
            {
            for (const auto& file: files)
                {   
                if (m_check_states[file] == Qt::Unchecked)
                    {
                    unchecked_file = file;
                    break;
                    }
                }
            if (unchecked_file.size() == 0)
                {
                auto err_msg = tr("Expected unchecked file in %1").arg(cb_represent(files));
                ABORT(err_msg)
                }
            }

        QStringList new_filelist;
        for (const auto& file : files)
            {
            if (m_check_states[file] != Qt::Checked)
                {
                new_filelist.append(file);
                continue;
                }

            bool    action_succes = false;
            QString msg_success;
            QString msg_fail;

            switch (action)
                {
                case action_delete :
                    action_succes = QFile::remove(file);
                    msg_success    = tr("Deleted: ");
                    msg_fail       = tr("Failed delete: ");
                    break;
                case action_link :
                    action_succes = QFile::remove(file);
                    action_succes = action_succes and cb_hardlink(unchecked_file, file);
                    msg_success    = tr("Hardlinked: ");
                    msg_fail       = tr("Failed hardlink: ");
                    break;
                }

            if (action_succes)
                {
                QTextStream(&logfile_success) << msg_success << file << Qt::endl;
                m_action_success_count++;
                }
            else
                {
                new_filelist.append(file);
                QTextStream(&logfile_fail) << msg_fail << file << Qt::endl;
                m_action_fail_count++;
                }
            }

        if (action != action_link)
            {
            // Replace the m_key_dict entry with the probably reduced new_filelist.
            m_key_dict.remove(key);
            m_key_dict[key] = new_filelist;
            }
        }

    // Clean the m_key_dict from lists with less than 2 entries.
    for (const auto& key: m_key_dict.keys())
        {
        const auto& files    = m_key_dict[key];
        const auto  nr_files = files.size();
        if (nr_files < 2)
            {
            m_key_dict.remove(key);
            }
        }

    const auto do_recalculate_times_and_inodes = (action == action_link);
    cb_set_result(do_recalculate_times_and_inodes);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_result_model::cb_log_key_dict() const
    {
    qInfo() << __PRETTY_FUNCTION__;

    for (const auto &key : m_key_dict.keys())
        {
        const auto& files    = m_key_dict[key];
        const auto  nr_files = files.size();
        qInfo() << "ResultModel"
                << QString("m_key_dict[%1](%2) : %3)")
                   .arg(key, 80)
                   .arg(nr_files, 3)
                   .arg(cb_represent(files));
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


cb_result_model::~cb_result_model() 
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
