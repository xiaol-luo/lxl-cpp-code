#include "LuaScriptDemo.h"
#include "Common.h"
#include "LuaScript.h"

LuaScriptDemo::LuaScriptDemo(LuaScript *luaScript) : LuaScriptObject(luaScript)
{


}
LuaScriptDemo::~LuaScriptDemo()
{

}

const luaL_Reg* LuaScriptDemo::GetLuaRegTable() 
{ 
	return m_luaRegTable; 
}

bool LuaScriptDemo::NewMetatable()
{
	bool ret  = false;
	lua_State *pLua = m_luaScript->GetLuaState();

	KG_EXPECT_TRUE(pLua);
	lua_newtable(pLua);
	lua_pushvalue(pLua, -1);
	lua_setfield(pLua, -2, TABLE_INDEX_FUNCTION);
	luaL_setfuncs(pLua, m_luaRegTable, 0);
	KG_EXPECT_TRUE(LuaScriptObject::NewMetatable());
	lua_setmetatable(pLua, -2);

	ret = true;
exit0:
	return ret;
}

int LuaScriptDemo::Lua_SetName(lua_State *pLua)
{
	bool ret = false;

	LuaScriptObject *pObject = NULL;
	const char *pName = NULL;

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(lua_gettop(pLua) == 2);
	KG_EXPECT_TRUE(lua_isstring(pLua, -1));
	pObject = ToLuaScriptObject<LuaScriptObject>(pLua, -2);
	KG_EXPECT_TRUE(pObject);
	pName = lua_tostring(pLua, -1);
	KG_EXPECT_TRUE(pName);
	KG_EXPECT_TRUE(pName[0]);
	pObject->SetName(pName);

exit0:
	return 0;
}


const luaL_Reg LuaScriptDemo::m_luaRegTable[] = 
{
	{"SetName",		LuaScriptDemo::Lua_SetName},
	{NULL,          NULL},
};
