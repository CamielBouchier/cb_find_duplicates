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

#include "cb_filesystem_model.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_filesystem_model::cb_filesystem_model() : QFileSystemModel(nullptr)
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Overriding QFileSystemModel. Adding checkboxes (tristate) for column 0.

Qt::ItemFlags cb_filesystem_model::flags(const QModelIndex& the_index) const
    {
    auto rv = QFileSystemModel::flags(the_index);
    if (the_index.column() == 0)
        {
        rv |= Qt::ItemIsUserCheckable;
        }
    return rv;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QVariant cb_filesystem_model::data(const QModelIndex& the_index, int role) const
    {
    if (not (role == Qt::CheckStateRole and the_index.column() == 0))
        {
        return QFileSystemModel::data(the_index, role);
        }
    if (m_check_states.contains(the_index))
        {
        return m_check_states.value(the_index); // Simple case : we have the check_state already.
        }
    else
        {
        // Here we maybe have to propagate down from the parent.
        auto parent_index = the_index.parent();
        if (m_prop_down_states.contains(parent_index))
            {
            // Yes, we have to propagate down.
            auto state = m_prop_down_states.value(parent_index);
            // setData is not const. But OK : class stays conceptually const. It's 'caching' like.
            const_cast <cb_filesystem_model*> (this)->
                setData(the_index, state, Qt::CheckStateRole);
            return state;
            }
        else
            {
            return Qt::Unchecked; // No. Then unchecked.
            }
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool cb_filesystem_model::setData(const QModelIndex& the_index, const QVariant& value, int role)
    {
    if (not (role == Qt::CheckStateRole and the_index.column() == 0))
        {
        return QFileSystemModel::setData(the_index,value,role);
        }

    cb_set_node(the_index,value);

    // Upward collected from childs.
    auto parent_index = the_index.parent();
    if (parent_index != QModelIndex())
        {
        auto status = cb_get_child_status(parent_index);
        setData(parent_index, status, Qt::CheckStateRole);
        }

    return true;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Convenience : setting node to a value.

void cb_filesystem_model::cb_set_node(const QModelIndex& the_index, const QVariant& value)
    {
    auto check_value = static_cast <Qt::CheckState> (value.toInt());
    m_check_states[the_index] = check_value;
    if (check_value == Qt::PartiallyChecked)
        {
        if (m_prop_down_states.contains(the_index))
            {
            m_prop_down_states.remove(the_index); // Partial check is never pushed down.
            }
        }
    else
        {
        // Checked/Unchecked is marked for pushing down.
        m_prop_down_states[the_index] = check_value;
        // Pushing down immediately if open nodes.
        for (int child_row = 0; child_row < rowCount(the_index); child_row++)
            {
            auto child_index = index(child_row, 0, the_index);
            cb_set_node(child_index, value);
            }
        }

    emit dataChanged(the_index, the_index); // Signal the change.
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Convenience, determining if a parent node should be checked/unchecked/partially

Qt::CheckState cb_filesystem_model::cb_get_child_status(const QModelIndex& the_index) const
    {
    bool all_checked   = true;
    bool all_unchecked = true;
    for (int child_row = 0; child_row < rowCount(the_index); child_row++)
        {
        auto child_index  = index(child_row, 0, the_index);
        auto child_status = data(child_index, Qt::CheckStateRole);
        if (child_status != Qt::Unchecked)
            {
            all_unchecked = false;
            }
        if (child_status != Qt::Checked)
            {
            all_checked = false;
            }
        if (not (all_checked or all_unchecked))
            {
            break;
            }
        }
    if (all_checked)
        {
        return Qt::Checked;
        }
    if (all_unchecked)
        {
        return Qt::Unchecked;
        }
    return Qt::PartiallyChecked;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Get the selected directories.
// Filtering out the implied ones (i.e. childs of already selected ones).

const QStringList cb_filesystem_model::cb_get_selected() const
    {
    QStringList selected;
    for (const auto& the_index: m_check_states.keys())
        {
        auto parent_index =the_index.parent();
        if ((parent_index != QModelIndex())       and
            m_check_states.contains(parent_index) and
            (m_check_states.value(parent_index) == Qt::Checked))
            {
            continue; // Skip this one. Implied by parent.
            }
        else if (m_check_states.contains(the_index) and
                 (m_check_states.value(the_index) == Qt::Checked))
            {
            selected.append(filePath(the_index));
            }
        }
    return selected;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_filesystem_model::~cb_filesystem_model()
    {
    qInfo() << __PRETTY_FUNCTION__;
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
