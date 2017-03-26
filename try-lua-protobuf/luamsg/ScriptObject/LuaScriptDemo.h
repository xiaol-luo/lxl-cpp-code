#ifndef __LUA_SCRIPT_DEMO_H__
#define __LUA_SCRIPT_DEMO_H__

#include "LuaScriptObject.h"

class LuaScriptDemo : public LuaScriptObject
{
public:
	LuaScriptDemo(LuaScript *luaScript);
	virtual ~LuaScriptDemo();

	virtual const luaL_Reg* GetLuaRegTable();
	virtual bool NewMetatable();

protected:
	static int Lua_SetName(lua_State *pLua);

private:
	static const luaL_Reg m_luaRegTable[];
};

#endif
