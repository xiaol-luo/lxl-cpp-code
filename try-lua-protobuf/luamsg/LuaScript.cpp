#include "LuaScript.h"
#include "LuaUtil.h"
#include "Common.h"
#include "LuaProtobuf.h"
#include "ScriptObject/LuaScriptObject.h"
#include "Log/Log.h"

extern const luaL_Reg local_LuaScriptRegTable[];
const static char *SCRIPT_LUASCRIPT_PTR = "SCRIPT_LUASCRIPT_PTR";
const char *SCRIPT_LUASCRIPT_TABLE = "___SCRIPT_SYSTEM_LUASCRIPT_TABLE___*$!@#$%^&*()_+";

LuaScript::LuaScript() : m_mainTable(nullptr), m_dirPath(nullptr), m_pLua(nullptr), m_errFunRef(LUA_REFNIL),
	m_objectHead(nullptr), m_objectTail(nullptr)
{
	m_objectHead = new LuaScriptObject(nullptr);
	m_objectTail = new LuaScriptObject(nullptr);
	m_objectHead->Append(m_objectTail);
}

LuaScript::~LuaScript()
{
	this->ClearRes();
}

LuaScript * LuaScript::GetThis(lua_State *pLua)
{
	LuaScript *pLuaScript = nullptr;
	int top = lua_gettop(pLua);

	lua_getfield(pLua, LUA_REGISTRYINDEX, SCRIPT_LUASCRIPT_TABLE);
	if (lua_istable(pLua, -1))
	{
		lua_getfield(pLua, -1, SCRIPT_LUASCRIPT_PTR);
		if (lua_islightuserdata(pLua, -1))
			pLuaScript = (LuaScript *)lua_touserdata(pLua, -1);
	}
	lua_settop(pLua, top);

	return pLuaScript;
}

bool LuaScript::Init(const char *dirPath, const char *mainTable)
{
	if (nullptr == dirPath || 0 == dirPath[0] ||
		nullptr == mainTable || 0 == mainTable[0])
		return false;

	size_t dirPathLen = strlen(dirPath);
	size_t mainTableLen = strlen(mainTable);

	m_dirPath = (char *)malloc(dirPathLen + 1);
	m_mainTable = (char *)malloc(mainTableLen + 1);

	if (nullptr == m_dirPath || nullptr == m_mainTable)
		return false;

	memcpy(m_dirPath, dirPath, dirPathLen);
	m_dirPath[dirPathLen] = 0;
	memcpy(m_mainTable, mainTable, mainTableLen);
	m_mainTable[mainTableLen] = 0;

	for (int i = strlen(m_dirPath); i >= 0; --i)
	{
		if ('\\' == m_dirPath[i] || '/' == m_dirPath[i])
		{
			m_dirPath[i] = 0;
			continue;
		}

		break;
	}

	m_pLua = luaL_newstate();
	if (nullptr == m_pLua)
		return false;

	{
		LuaProtobuf *luaProtobuf = LuaProtobufManager::Instance()->CreateLuaProtobuf(m_pLua);
		if (nullptr == luaProtobuf)
			return false;

		luaProtobuf->AddMapPath("", m_dirPath);
		LuaProtobuf::RegLuaProtobufFunction(m_pLua);
	}

	{
		int topIdx = lua_gettop(m_pLua);
		lua_newtable(m_pLua);
		lua_pushlightuserdata(m_pLua, this);
		lua_setfield(m_pLua, -2, SCRIPT_LUASCRIPT_PTR);
		lua_setfield(m_pLua, LUA_REGISTRYINDEX, SCRIPT_LUASCRIPT_TABLE);

		lua_getfield(m_pLua, LUA_REGISTRYINDEX, SCRIPT_LUASCRIPT_TABLE);
		if (!lua_istable(m_pLua, - 1))
			return false;

		lua_settop(m_pLua, topIdx);
	}

	luaL_openlibs(m_pLua);

	{
		const char *SCRIPT_ERROR_HANDLER = "ScriptErrorHandler";
		int topIdx = lua_gettop(m_pLua);
		lua_pushglobaltable(m_pLua);
		luaL_setfuncs(m_pLua, local_LuaScriptRegTable, 0);
		if (LuaUtil::PushCallOnStack(m_pLua, "", SCRIPT_ERROR_HANDLER))
		{
			m_errFunRef = luaL_ref(m_pLua, LUA_REGISTRYINDEX);
		}
		lua_settop(m_pLua, topIdx);

		if (LUA_REFNIL == m_errFunRef)
			return false;
	}

	{
		int topIdx = lua_gettop(m_pLua);
		if (!LuaUtil::PushCallOnStack(m_pLua, m_errFunRef))
			return false;

		char buff[2048] = { 0 };
		Compat::SNPrintf(buff, sizeof(buff), "%s/%s.lua", dirPath, mainTable);

		int errFuncIdx = lua_gettop(m_pLua);
		if (!LuaUtil::LoadFile(m_pLua, buff, errFuncIdx))
			return false;

		lua_settop(m_pLua, topIdx);
	}

	lua_settop(m_pLua, 0);
	return true;
}

bool LuaScript::Uninit()
{
	this->ClearRes();
	return true;
}

void LuaScript::Activate()
{
	static int64_t now_ms = 0;

	++ now_ms;

	LuaScriptObject *pObject = m_objectHead->GetNext();
	while (pObject != m_objectTail)
	{
		LuaScriptObject *pNextObject = pObject->GetNext();
		pObject->Activate(now_ms);

		if (pObject->IsWaitDispose())
		{
			pObject->Detach();
			pObject->Dispose();
		}

		pObject = pNextObject;
	}
}

void LuaScript::AppendScriptObject(LuaScriptObject *scriptObj)
{
	if (nullptr == scriptObj)
		return;

	m_objectTail->Insert(scriptObj);
}

bool LuaScript::ExcuteMainTable(const char *funcName, int resultCount, const char *format, ...)
{
	int errFuncIdx = 0;
	this->PushErrorOnStack(errFuncIdx);

	va_list argList;
	va_start(argList, format);
	bool ret = LuaUtil::Call(m_pLua, m_mainTable, funcName, resultCount, errFuncIdx, format, argList);
	va_end(argList);

	if (errFuncIdx > 0)
	{
		lua_remove(m_pLua, errFuncIdx);
	}

	return ret;
}

bool LuaScript::LoadScripts(const char *relatePath)
{
	if (nullptr == relatePath || 0 == relatePath[0])
		return false;

	char buff[2048] = { 0 };
	Compat::SNPrintf(buff, sizeof(buff), "%s/%s", m_dirPath, relatePath);
	int topIdx = lua_gettop(m_pLua);
	int errFuncIdx = 0;
	this->PushErrorOnStack(errFuncIdx);
	bool ret = LuaUtil::LoadScripts(m_pLua, buff, errFuncIdx);
	lua_settop(m_pLua, topIdx);

	return ret;
}

void LuaScript::ClearRes()
{
	if (nullptr != m_dirPath)
	{
		free(m_dirPath);
		m_dirPath = nullptr;
	}

	if (nullptr != m_mainTable)
	{
		free(m_mainTable);
		m_mainTable = nullptr;
	}

	LuaScriptObject *pObject = m_objectHead->GetNext();
	while (pObject != m_objectTail)
	{
		LuaScriptObject *pNextObject = pObject->GetNext();
		pObject->Detach();
		pObject->Dispose();
		pObject = pNextObject;
	}

	LuaProtobufManager::Instance()->EraseLuaProtobuf(m_pLua);

	if (nullptr != m_pLua)
	{
		lua_close(m_pLua);
		m_pLua = nullptr;
	}
}

bool LuaScript::PushErrorOnStack(int &locateIdx)
{
	locateIdx = 0;

	if (!LuaUtil::PushCallOnStack(m_pLua, m_errFunRef))
		return false;

	locateIdx = lua_gettop(m_pLua);
	return true;
}

int Lua_ScriptErrorHandler(lua_State *pLua)
{
	int topIdx = lua_gettop(pLua);

	if (lua_isstring(pLua, -1))
	{
		const char *errStr = lua_tostring(pLua, -1);
		LogDebug(LogConsole, "{0}\n", errStr);
	}

	return 0;
}

extern int Lua_LoadScripts(lua_State *pLua)
{
	int ret = false;

	do
	{
		int paramCount = lua_gettop(pLua);
		if (paramCount <= 0)
		{
			LogDebug(LogConsole, "Lua_LoadScripts paramter need {0}, but got %d, \n", 1, paramCount);
			break;
		}
		if (!lua_isstring(pLua, -1))
		{
			LogDebug(LogConsole, "Lua_LoadScripts paramter need string");
			break;
		}

		LuaScript *pLuaScript = LuaScript::GetThis(pLua);
		const char *filePath = lua_tostring(pLua, -1);

		if (pLuaScript)
			ret = pLuaScript->LoadScripts(filePath);
		else
			ret = LuaUtil::LoadScripts(pLua, filePath);

	} while (false);

	

	lua_pushboolean(pLua, ret);
	return 1;
}

const static luaL_Reg local_LuaScriptRegTable[] =
{
	{ "ScriptErrorHandler", Lua_ScriptErrorHandler },
	{ "LoadScripts", Lua_LoadScripts },
	{ NULL, NULL },
};
