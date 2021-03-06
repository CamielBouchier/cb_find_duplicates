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

#include "cb_abort.h"
#include "cb_lua_selector.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

static const luaL_Reg lua_libs[] =
    {
        {"base"         , luaopen_base},
        {"io"           , luaopen_io},
        {"string"       , luaopen_string},
        {nullptr        , nullptr}
    };

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_lua_selector::cb_lua_selector()
    {
    qInfo() << __PRETTY_FUNCTION__;

    m_L = luaL_newstate();
    if (not m_L)
        {
        auto err_msg = tr("Could not open new Lua state.");
        ABORT(err_msg);
        }
    const luaL_Reg* lib = lua_libs;
    for(; lib->func != nullptr; lib++)
        {
        luaL_requiref(m_L, lib->name, lib->func, 1);
        lua_settop(m_L,0); // Removing all stak elements.
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_lua_selector::cb_call_script(const QString&         script_name,
                                     const QStringList&     files,
                                     const QList<uint32_t>& mtimes,
                                     const QList<uint32_t>& ctimes,
                                     const QList<uint64_t>& inodes,
                                     const int&             size,
                                           QList <bool>&    selected,
                                           bool&            ok,
                                           QString&         message)

    {
    // qInfo() << __PRETTY_FUNCTION__;
    //         << "script_name:"
    //         << script_name;

    if ((selected.size() != files.size())  or
        (selected.size() != mtimes.size()) or
        (selected.size() != ctimes.size()) or
        (selected.size() != inodes.size()) )
        {
        auto err_msg = tr("Inconsistent sizes: %1, %2, %3, %4, %5")
                       .arg(files.size())
                       .arg(mtimes.size())
                       .arg(ctimes.size())
                       .arg(inodes.size())
                       .arg(selected.size());
        ABORT(err_msg);
        }

    ok = true;
    if (luaL_loadfile(m_L, qPrintable(script_name)) or lua_pcall(m_L, 0, 0, 0))
        {
        ok = false;
        message = lua_tostring(m_L,-1);
        return;
        }

    lua_getglobal(m_L, "select");

    // 4 first table arguments : files,mtimes,ctimes,inodes
    for (int arg=0; arg<4; arg++)
        {
        lua_createtable(m_L, files.size(), 0);
        for (int key=0; key<files.size(); key++)
            {
            lua_pushinteger(m_L, key+1);
            switch (arg)
                {
                case 0 :
                    lua_pushstring(m_L, qPrintable(files.at(key)));
                    break;
                case 1 :
                    lua_pushinteger(m_L, mtimes.at(key));
                    break;
                case 2 :
                    lua_pushinteger(m_L, ctimes.at(key));
                    break;
                case 3 :
                    lua_pushinteger(m_L, inodes.at(key));
                    break;
                }
            lua_settable(m_L, -3);
            }
        }
    // Last argument : size
    lua_pushinteger(m_L, size);

    ok = true;
    if (lua_pcall(m_L, 5, LUA_MULTRET, 0))
        {
        ok = false;
        message = lua_tostring(m_L, -1);
        return;
        }
    int top = lua_gettop(m_L);
    if (lua_type(m_L, top) != LUA_TTABLE)
        {
        ok = false;
        message = "Script did not return a table";
        return;
        }
    lua_pushnil(m_L);
    while (lua_next(m_L, top) != 0)
        {
        if ((lua_type(m_L, -2) != LUA_TNUMBER) or (lua_type(m_L, -1) != LUA_TBOOLEAN))
            {
            ok = false;
            message = tr("Script did not return [number]:boolean table, but [%1]:%2 table")
                      .arg(lua_typename(m_L, lua_type(m_L, -2)))
                      .arg(lua_typename(m_L, lua_type(m_L, -1)));
            return;
            }
        int  idx = static_cast <int> (lua_tonumber(m_L, -2));
        bool val = lua_toboolean(m_L, -1);
        if (idx < 1 or idx > files.size())
            {
            ok = false;
            message = tr("Returned index (%1) is out of range (0 < index < %2)")
                      .arg(idx)
                      .arg(files.size());
            return;
            }
        selected[idx-1] = val;
        lua_pop(m_L, 1);
        }

    // Did the script not select all values ?
    if (-1 == selected.indexOf(false))
        {
        ok = false;
        message = tr("Script did select all values. At least one should be not selected.");
        return;
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void cb_lua_selector::cb_get_script_info(const QString& script_name,
                                               QString& title,
                                               QString& description,
                                               bool&    ok,
                                               QString& message)
    {
    // qInfo() << __PRETTY_FUNCTION__
    //         << script_name
    //         << title
    //         << description
    //         << ok
    //         << message;

    ok = true;
    if (luaL_loadfile(m_L, qPrintable(script_name)) or lua_pcall(m_L, 0, 0, 0))
        {
        ok = false;
        message = lua_tostring(m_L, -1);
        return;
        }

    struct field { const char*    field_name;
                         bool     is_function;
                         QString& value;};
    QString dummy;
    const field fields[] =
        {
            { "title"      , false , title       },
            { "description", false , description },
            { "select"     , true  , dummy       },
            { nullptr      , false , dummy       }
        };
    const field* x = fields;
    for(; x->field_name != nullptr; x++)
        {
        lua_getglobal(m_L, x->field_name);
        if (not x->is_function and not lua_isstring(m_L, -1))
            {
            ok = false;
            message = tr("Cannot read '%1' as string.").arg(x->field_name);
            return;
            }
        else if (x->is_function and not lua_isfunction(m_L,-1))
            {
            ok = false;
            message = tr("Cannot find '%1' as function.").arg(x->field_name);
            return;
            }
        if (not x->is_function)
            {
            x->value = tr(lua_tostring(m_L, -1));
            }
        }
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cb_lua_selector::~cb_lua_selector()
    {
    qInfo() << __PRETTY_FUNCTION__;
    lua_close(m_L);
    }

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// vim: syntax=cpp ts=4 sw=4 sts=4 sr et columns=100
