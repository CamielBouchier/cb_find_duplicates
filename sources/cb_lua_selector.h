//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// $BeginLicense$
// $EndLicense$
//
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#pragma once

#include <QtCore>
#include <lua.hpp>
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class cb_lua_selector : public QObject
    {
    Q_OBJECT
  
    public :
  
      cb_lua_selector();
      ~cb_lua_selector();

      void cb_call_script(const QString&         script_name, 
                          const QStringList&     files,
                          const QList<uint32_t>& mtimes,
                          const QList<uint32_t>& ctimes,
                          const QList<uint64_t>& inodes,
                          const int&             size, 
                                QList<bool>&     selected,
                                bool&            ok,
                                QString&         message);

      void cb_get_script_info(const QString& script_name, 
                                    QString& title,
                                    QString& description,
                                    bool&    ok,
                                    QString& message);

    private :

      lua_State* m_L;

    };
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
