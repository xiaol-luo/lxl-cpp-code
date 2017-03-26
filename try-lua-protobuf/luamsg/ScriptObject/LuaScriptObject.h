#ifndef __LUA_SCRIPT_OBJECT_H__
#define __LUA_SCRIPT_OBJECT_H__

#include <map>
#include <stdint.h>
extern "C" 
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

const static char *TABLE_INDEX_FUNCTION = "__index";

class LuaScript;

class LuaScriptObject
{
public:
	LuaScriptObject(LuaScript *luaScript);
	virtual ~LuaScriptObject();

	void Insert(LuaScriptObject *pObject);
	void Append(LuaScriptObject *pObject);
	void Detach();
	LuaScriptObject* GetPre() { return m_pre; }
	LuaScriptObject* GetNext() {return m_next;}

	virtual int Activate(int64_t time_ms);
	bool Dispose();
	const std::string & GetName() { return m_name; };
	void SetName(std::string name) { m_name = name; }
	int IsWaitDispose() { return m_isWaitDispose; }
	void SetWaitDispose() { m_isWaitDispose = true; }
	void ClearLuaUserData();
	int GetEvent(const char *eventName);
	bool AddEvent(const char *eventName, int ref);
	void RemoveEvent(const char *eventName);
	void ClearEvent();

	bool CreateLuaObject();
	static LuaScriptObject* GetObject(lua_State *pLua, int index);

protected:
	LuaScript *m_luaScript;

private:
	LuaScriptObject *m_pre;
	LuaScriptObject *m_next;

protected:
	bool SetMetatable(int index);
	virtual bool NewMetatable();
	virtual const luaL_Reg* GetLuaRegTable();
	virtual bool Release();

	LuaScriptObject **m_ppLuaUserData;
	int m_isWaitDispose;

	typedef std::map<std::string, int> EventMap;
	typedef EventMap::iterator EventMapIt;
	std::map<std::string, int> m_eventMap;
	std::string m_name;

	static int Lua_SetName(lua_State *pLua);
	static int Lua_RegisterEvent(lua_State *pLua);
	static int Lua_UnregisterEvent(lua_State *pLua);
	static int Lua_ClearEvent(lua_State *pLua);
	static int Lua_Release(lua_State *pLua);

private:
	static const luaL_Reg m_luaRegTable[];
};

template<typename className>
static inline className* ToLuaScriptObject(lua_State *pLua, int index)
{
	return dynamic_cast<className *>(className::GetObject(pLua, index));
};

#endif
