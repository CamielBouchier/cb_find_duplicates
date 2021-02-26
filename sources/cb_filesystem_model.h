//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QFileSystemModel>
#include <QtCore>
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class cb_filesystem_model : public QFileSystemModel
    {
    Q_OBJECT
  
    public :
  
      	cb_filesystem_model();
      	~cb_filesystem_model();

      	const QStringList cb_get_selected() const;
  
    protected :
  
      	Qt::ItemFlags flags(const QModelIndex& index) const override;

      	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

      	bool setData(const QModelIndex& index, 
                     const QVariant&    value, 
                     int          role = Qt::EditRole) override;
    private :

      	void cb_set_node(const QModelIndex& index, const QVariant& value);

      	Qt::CheckState cb_get_child_status(const QModelIndex& index) const;

      	QHash <QModelIndex, Qt::CheckState> m_check_states;
      	QHash <QModelIndex, Qt::CheckState> m_prop_down_states;
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
