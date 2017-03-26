#include <string>
#include <stdio.h>
#include <windows.h>
#include "GlobalDef.h"
#include "compat/Compat.h"
#include "LuaProtobuf.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "LuaUtil.h"
#include "LuaScript.h"
#include "ScriptObject/LuaScriptObject.h"
#include "ScriptObject/LuaScriptDemo.h"
#include "Log/Log.h"

extern int Lua_CreateLuaScriptObject(lua_State *pLua);
extern int Lua_CreateLuaScriptDemo(lua_State *pLua);

const static luaL_Reg main_LuaScriptRegTable[] =
{
	{ "CreateLuaScriptObject", Lua_CreateLuaScriptObject },
	{ "CreateLuaScriptDemo", Lua_CreateLuaScriptDemo },
	
	{ NULL, NULL },
};


int main(int argc, char **argv)
{
	LogInit();

	char cwdBuff[512] = { 0 };
	Compat::GetCurrWorkDir(cwdBuff, sizeof(cwdBuff) / sizeof(cwdBuff[0]));

	char pathBuff[512] = { 0 };

	Compat::SNPrintf(pathBuff, sizeof(pathBuff), "%s/%s", cwdBuff, "proto");
	
	Compat::SNPrintf(pathBuff, sizeof(pathBuff), "%s/script/", cwdBuff);

	const char *SCRIPT_MAIN_TABLE = "Main";
	
	while (true)
	{
		LuaScript luaScript;
		bool ret = luaScript.Init(pathBuff, SCRIPT_MAIN_TABLE);
		if (ret)
		{
			lua_pushglobaltable(luaScript.GetLuaState());
			luaL_setfuncs(luaScript.GetLuaState(), main_LuaScriptRegTable, 0);

			bool result = luaScript.ExcuteMainTable(SCRIPT_FUNCTION_NAME_INIT, 1);
			if (result)
			{
				lua_State *pLua = luaScript.GetLuaState();
				if (lua_isstring(pLua, -1))
				{
					LogDebug(LogConsole, "ExcuteMainTable %s return {0} \n", SCRIPT_FUNCTION_NAME_INIT, lua_tostring(pLua, -1));
				}
				lua_pop(pLua, 1);
			}

			luaScript.ExcuteMainTable(SCRIPT_FUNCTION_NAME_ACTIVATE, 0);

			lua_gc(luaScript.GetLuaState(), LUA_GCSTOP, 0);

			luaScript.ExcuteMainTable(SCRIPT_FUNCTION_NAME_UNINIT, 0);
		}

		for (int i = 0; i < LogCount; ++i)
		{
			LogDebug(i, "LOG_DEBUG");
			LogInfo(i, "CONSOLE_LOG_INFO");
			LogWarn(i, "CONSOLE_LOG_WARNING");
			LogError(i, "CONSOLE_LOG_ERROR");
		}

		Compat::SleepMS(2 * 1000);
	}

	LogUninit();
}

int Lua_CreateLuaScriptObject(lua_State *pLua)
{
	LuaScript *luaScript = LuaScript::GetThis(pLua);

	LuaScriptObject *pObject = new LuaScriptObject(luaScript);
	pObject->CreateLuaObject();
	return 1;
}

int Lua_CreateLuaScriptDemo(lua_State *pLua)
{
	LuaScript *luaScript = LuaScript::GetThis(pLua);

	LuaScriptObject *pObject = new LuaScriptDemo(luaScript);
	pObject->CreateLuaObject();
	return 1;
}