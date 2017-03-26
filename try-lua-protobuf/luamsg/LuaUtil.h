#ifndef __LUA_UTIL_H__
#define __LUA_UTIL_H__

struct lua_State;
#include <stdarg.h>

const static char *LUA_FILE_SUFFIT = ".lua";
const static char *LUA_FILE_LIST_SUFFIT = ".txt";


namespace LuaUtil
{
	bool PushCallOnStack(lua_State *pLua, const char *table, const char *func);
	bool PushCallOnStack(lua_State *pLua, int ref);
	int PushArgs(lua_State *pLua, const char *format, ...); 
	int PushArgs(lua_State *pLua, const char *format, va_list argList); 
	bool Call(lua_State *pLua, const char *table, const char *func, int resultCount, int errFuncIdx, const char *format, va_list argList); 
	bool Call(lua_State *pLua, int ref, int resultCount, int errFuncIdx, const char *format, va_list argList);
	bool Call(lua_State *pLua, const char *table, const char *func, int resultCount, int errFuncIdx, const char *format = nullptr, ...);
	bool Call(lua_State *pLua, int ref, int resultCount, int errFuncIdx, const char *format = nullptr, ...);
	bool StackCall(lua_State *pLua, int paramCount, int resultCout, int errFunIdx);
	bool LoadFile(lua_State *pLua, const char *filePath, int errFunIdx = 0);
	bool LoadFileList(lua_State *pLua, const char *filePath, int errFunIdx = 0);
	bool LoadScripts(lua_State *pLua, const char *filePath, int errFunIdx = 0);
}

#endif