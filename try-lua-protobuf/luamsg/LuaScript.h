#ifndef __LUA_SCRIPT_H__
#define __LUA_SCRIPT_H__

#include <string>

class LuaScriptObject;

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

const static char *SCRIPT_FUNCTION_NAME_INIT = "OnInit";
const static char *SCRIPT_FUNCTION_NAME_ACTIVATE = "OnActivate";
const static char *SCRIPT_FUNCTION_NAME_UNINIT = "OnUninit";

extern const char *SCRIPT_LUASCRIPT_TABLE;


class LuaScript
{
public:
	LuaScript();
	~LuaScript();

	static LuaScript * GetThis(lua_State *pLua);

	bool Init(const char *dirPath, const char *mainTable);
	bool Uninit();
	void Activate();
	bool ExcuteMainTable(const char *funcName, int resultCount, const char *format = nullptr, ...);
	bool LoadScripts(const char *relatePath);
	int GetErrFuncRef() { return m_errFunRef; }
	lua_State * GetLuaState() { return m_pLua; }

	const char * GetDirPath() { return m_dirPath; }
	const char * GetFileDir(const char *releatePath);

	void AppendScriptObject(LuaScriptObject *scriptObj);

private:
	int m_state;

	char *m_mainTable;
	char *m_dirPath;
	lua_State *m_pLua;
	int m_errFunRef;

private:
	void ClearRes();
	bool PushErrorOnStack(int &locateIdx);

	LuaScriptObject *m_objectHead;
	LuaScriptObject *m_objectTail;
};

#endif
