#include "LuaUtil.h"
#include <string.h>
#include <fstream>
#include "Common.h"
#include "Log/Log.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

bool LuaUtil::PushCallOnStack(lua_State *pLua, int ref)
{
	if (nullptr == pLua || LUA_NOREF == ref || LUA_REFNIL == ref)
		return false;

	int topIdx = lua_gettop(pLua);

	lua_rawgeti(pLua, LUA_REGISTRYINDEX, ref);
	if (!lua_isfunction(pLua, -1))
	{
		lua_settop(pLua, topIdx);
		return false;
	}

	return true;
}

bool LuaUtil::PushCallOnStack(lua_State *pLua, const char *table, const char *func)
{
	if (nullptr == pLua || nullptr == func || 0 == func[0])
		return false;
	
	bool ret = false;
	int topIdx = lua_gettop(pLua);
	
	if (nullptr != table && 0 != table[0])
	{
		lua_getglobal(pLua, table);
		if (lua_istable(pLua, -1))
		{
			lua_getfield(pLua, -1, func);
			ret = lua_isfunction(pLua, -1);
			lua_remove(pLua, -2);
		}
	}
	else
	{
		lua_getglobal(pLua, func);
		ret = lua_isfunction(pLua, -1);
	}

	if (!ret)
		lua_settop(pLua, topIdx);
		
	return ret;
}

int LuaUtil::PushArgs(lua_State *pLua, const char *format, ...)
{
	va_list argList;
	va_start(argList, format);
	int argCount = LuaUtil::PushArgs(pLua, format, argList);
	va_end(argList);

	return argCount;
}

int LuaUtil::PushArgs(lua_State *pLua, const char *format, va_list argList)
{
	if (nullptr == pLua)
		return -1;

	if (nullptr == format || 0 == format[0])
		return 0;

	int argCount = strlen(format);

	for (int i = 0; i < argCount; ++i)
	{
		switch (i)
		{
			case 'i':
				lua_pushnumber(pLua, va_arg(argList, int));
				break;
			case 'b':
				lua_pushboolean(pLua, va_arg(argList, bool));
				break;
			case 's':
				lua_pushstring(pLua, va_arg(argList, const char *));
				break;
		}
	}

	return argCount;
}


bool LuaUtil::Call(lua_State *pLua, const char *table, const char *func, int resultCount, int errFuncIdx, const char *format, va_list argList)
{
	if (nullptr == pLua)
		return false;

	bool ret = false;
	int topIdx = lua_gettop(pLua);

	if (LuaUtil::PushCallOnStack(pLua, table, func))
	{
		int argCount = LuaUtil::PushArgs(pLua, format, argList);
		if (argCount >= 0)
		{
			if (LuaUtil::StackCall(pLua, argCount, resultCount, errFuncIdx))
				ret = true;
		}
	}

	if (!ret)
		lua_settop(pLua, topIdx);

	return ret;
}

bool LuaUtil::Call(lua_State *pLua, int ref, int resultCount, int errFuncIdx, const char *format, va_list argList)
{
	if (nullptr == pLua || LUA_NOREF == ref || LUA_REFNIL == ref)
		return false;

	int topIdx = lua_gettop(pLua);
	bool ret = false;

	if (LuaUtil::PushCallOnStack(pLua, ref))
	{
		int argCount = LuaUtil::PushArgs(pLua, format, argList);
		if (argCount >= 0)
		{
			if (LuaUtil::StackCall(pLua, argCount, resultCount, errFuncIdx))
				ret = true;
		}
	}

	if (!ret)
		lua_settop(pLua, topIdx);

	return ret;
}

bool LuaUtil::Call(lua_State *pLua, const char *table, const char *func, int resultCount, int errFuncIdx, const char *format, ...)
{
	va_list argList;
	va_start(argList, format);
	bool ret = LuaUtil::Call(pLua, table, func, resultCount, errFuncIdx, format, argList);
	va_end(argList);

	return ret;
}

bool LuaUtil::Call(lua_State *pLua, int ref, int resultCount, int errFuncIdx, const char *format, ...)
{
	va_list argList;
	va_start(argList, format);
	bool ret = LuaUtil::Call(pLua, ref, resultCount, errFuncIdx, format, argList);
	va_end(argList);

	return ret;
}

bool LuaUtil::StackCall(lua_State *pLua, int paramCount, int resultCout, int errFunIdx)
{
	if (nullptr == pLua || paramCount < 0 || resultCout < 0)
		return false;

	bool ret = false;
	int topIdx = lua_gettop(pLua);

	if (topIdx > paramCount && lua_isfunction(pLua, topIdx - paramCount))
	{
		if (LUA_OK == lua_pcall(pLua, paramCount, resultCout, errFunIdx))
			ret = true;
	}
	
	if (!ret)
	{
		if (lua_isstring(pLua, -1) && lua_gettop(pLua) > topIdx)
		{
			LogDebug(LogConsole, "LuaUtil::StackCall error {0} ", lua_tostring(pLua, -1));
			lua_settop(pLua, topIdx);
		}
	}
		

	return ret;
}

bool LuaUtil::LoadFile(lua_State *pLua, const char *filePath, int errFunIdx)
{
	int topIdx = lua_gettop(pLua);

	if (NULL == pLua || NULL == filePath || 0 == filePath[0])
		return false;

	if (!Util::IsSuffitMatch(filePath, LUA_FILE_SUFFIT))
		return false;

	if (LUA_OK != luaL_loadfile(pLua, filePath))
	{
		if (lua_isstring(pLua, -1) && lua_gettop(pLua) > topIdx)
		{
			LogDebug(LogConsole, "LuaUtil::LoadFile error {0} ", lua_tostring(pLua, -1));
			lua_settop(pLua, topIdx);
		}
		
		return false;
	}

	if (lua_pcall(pLua, 0, 0, errFunIdx))
	{
		if (0 == errFunIdx && lua_isstring(pLua, -1))
		{
			LogDebug(LogConsole, "LuaUtil::LoadFile error {0} ", lua_tostring(pLua, -1));
			lua_settop(pLua, topIdx);
		}
		return false;
	}

	return true;
}
bool LuaUtil::LoadFileList(lua_State *pLua, const char *filePath, int errFunIdx)
{
	if (NULL == pLua || NULL == filePath || 0 == filePath[0])
		return false;

	if (Util::IsSuffitMatch(filePath, LUA_FILE_SUFFIT))
	{
		LogDebug(LogConsole, "LuaUtil::LoadFileList suffix not match {0} , fileName{1} ", LUA_FILE_LIST_SUFFIT,filePath);
		return false;
	}

	std::ifstream fs(filePath);
	if (!fs.is_open())
	{
		LogDebug(LogConsole, "LuaUtil::LoadFileList open file error {0} ", filePath);
		return false;
	}

	int baseDirLen = 0;
	{
		int filePathLen = strlen(filePath);
		for (int i = 0; i < filePathLen; ++i)
		{
			if ('\\' == filePath[i] || '/' == filePath[i])
				baseDirLen = i;
		}
	}
	
	const static int PATH_BUFF_SIZE = 2048;
	char pathBuff[PATH_BUFF_SIZE] = { 0 };

	if (PATH_BUFF_SIZE <= baseDirLen)
		return false;

	memcpy(pathBuff, filePath, baseDirLen);

	const static int FILE_BUFF_SIZE = 10240 + 2;
	char fileBuff[FILE_BUFF_SIZE];

	int topIdx = lua_gettop(pLua);
	bool ret = true;

	while (!fs.eof())
	{
		char *fileName = NULL;

		fs.getline(fileBuff, FILE_BUFF_SIZE);

		for (int i = 0; 0 != fileBuff[i]; ++i)
		{
			char &c = fileBuff[i];

			bool abandomLine = false;
			bool hitEnd = false;

			switch (c)
			{
				case '#':
					c = 0;
					hitEnd = true;
					abandomLine = (NULL == fileName);
					break;
				case ' ':
					c = 0;
					break;
				case '\r':
					c = 0;
					break;
				case '\n':
					c = 0;
					hitEnd = true;
					break;

				default:
					if (!fileName)
						fileName = fileBuff + i;
					break;
			}

			if (abandomLine)
			{
				fileName = NULL;
				break;
			}

			if (hitEnd)
				break;
		}

		if (fileName)
		{
			Compat::SNPrintf(pathBuff + baseDirLen, PATH_BUFF_SIZE - baseDirLen, "/%s", fileName);

			if (Util::IsSuffitMatch(pathBuff, LUA_FILE_SUFFIT))
			{
				if (!LoadFile(pLua, pathBuff, errFunIdx))
				{
					ret = false;
					break;
				}
			}
			else if (Util::IsSuffitMatch(pathBuff, LUA_FILE_LIST_SUFFIT))
			{
				if (!LoadFileList(pLua, pathBuff))
				{
					ret = false;
					break;
				}
			}
			else
			{
				LogDebug(LogConsole, "LuaUtil::LoadFileList unknow suffix , fileName{0} ", fileName);
				return false;
				break;
			}
		}
	}

	fs.close();
	return ret;
}

bool LuaUtil::LoadScripts(lua_State *pLua, const char *filePath, int errFunIdx)
{
	if (Util::IsSuffitMatch(filePath, LUA_FILE_LIST_SUFFIT))
	{
		return LuaUtil::LoadFileList(pLua, filePath, errFunIdx);
	}
	if (Util::IsSuffitMatch(filePath, LUA_FILE_SUFFIT))
	{
		return LuaUtil::LoadFile(pLua, filePath, errFunIdx);
	}
	
	LogDebug(LogConsole, "LoadScripts suffit neight {0} nor {1}", LUA_FILE_SUFFIT, LUA_FILE_LIST_SUFFIT);
	return false;
	
}

