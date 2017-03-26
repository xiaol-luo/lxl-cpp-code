#include "LuaScriptObject.h"
#include <assert.h>
#include "Common.h"
#include "LuaScript.h"

LuaScriptObject::LuaScriptObject(LuaScript *luaScript) : m_luaScript(luaScript), m_pre(nullptr), m_next(nullptr),
	m_ppLuaUserData(nullptr), m_isWaitDispose(false)
{
	m_pre = nullptr;
	m_next = nullptr;

	if (nullptr != luaScript)
		m_luaScript->AppendScriptObject(this);
}


LuaScriptObject::~LuaScriptObject()
{
	this->Detach();
	this->ClearLuaUserData();
}

void LuaScriptObject::Insert(LuaScriptObject *pObject)
{
	assert(pObject->m_next == nullptr);
	assert(pObject->m_pre == nullptr);
	assert(pObject);

	pObject->m_pre = m_pre;
	pObject->m_next = this;

	if (m_pre)
		m_pre->m_next = pObject;

	m_pre = pObject;
}

void LuaScriptObject::Append(LuaScriptObject *pObject)
{
	assert(pObject->m_next == nullptr);
	assert(pObject->m_pre == nullptr);
	assert(pObject);

	pObject->m_pre = this;
	pObject->m_next = m_next;

	if (m_next)
		m_next->m_pre = pObject;

	m_next = pObject;
}

void LuaScriptObject::Detach()
{
	if (m_pre)
		m_pre->m_next = m_next;

	if (m_next)
		m_next->m_pre = m_pre;

	m_pre = nullptr;
	m_next = nullptr;
}

int LuaScriptObject::Activate(int64_t time_ms)
{
	return true;
}

bool LuaScriptObject::Dispose()
{
	m_isWaitDispose = false;
	this->Release();

	return true;
}

void LuaScriptObject::ClearLuaUserData() 
{
	if (m_ppLuaUserData)
	{
		*m_ppLuaUserData = nullptr;
		m_ppLuaUserData = nullptr;
	}
}

int LuaScriptObject::GetEvent(const char *eventName)
{
	int ref = LUA_REFNIL;
	EventMapIt it = m_eventMap.find(eventName);
	if (m_eventMap.end() != it)
		ref = it->second;

	return ref;
}

bool LuaScriptObject::AddEvent(const char *eventName, int ref)
{
	if (nullptr == eventName || 0 == eventName[0])
		return false;

	if (LUA_REFNIL != this->GetEvent(eventName))
	{
		m_eventMap[eventName] = ref;
		return true;
	}

	return false;
}

void LuaScriptObject::RemoveEvent(const char *eventName)
{
	int ref = this->GetEvent(eventName);

	if (LUA_REFNIL != ref)
	{
		m_eventMap.erase(eventName);
		luaL_unref(m_luaScript->GetLuaState(), LUA_REGISTRYINDEX, ref);
	}
}

void LuaScriptObject::ClearEvent()
{
	lua_State *pLua = m_luaScript->GetLuaState();

	for (EventMapIt it = m_eventMap.begin(); m_eventMap.end() != it; ++ it)
	{
		int ref = it->second;
		if (LUA_NOREF == ref || LUA_REFNIL == ref)
			continue;

		luaL_unref(pLua, LUA_REGISTRYINDEX, ref);
	}

	m_eventMap.clear();
}

bool LuaScriptObject::CreateLuaObject()
{
	bool ret = false;
	LuaScriptObject **ppUserData = nullptr;

	lua_State *pLua = m_luaScript->GetLuaState();
	int topIdx = lua_gettop(pLua);

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(nullptr == m_ppLuaUserData); // never create lua object before

	ppUserData = (LuaScriptObject **)lua_newuserdata(pLua, sizeof(LuaScriptObject *));
	KG_EXPECT_TRUE(ppUserData);
	KG_EXPECT_TRUE(this->SetMetatable(-1));

	*ppUserData = this;
	m_ppLuaUserData = ppUserData;
	ret = true;

exit0:
	if (!ret)
		lua_settop(pLua, -1);

	return ret;
}

LuaScriptObject* LuaScriptObject::GetObject(lua_State *pLua, int index)
{
	bool ret = false;
	LuaScriptObject *pScriptObject = NULL;
	LuaScriptObject **ppScriptObject = NULL;

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(lua_isuserdata(pLua, index));
	ppScriptObject = (LuaScriptObject **)lua_touserdata(pLua, index);
	KG_EXPECT_TRUE(ppScriptObject);
	pScriptObject = *ppScriptObject;

exit0:
	return pScriptObject;
}

bool LuaScriptObject::SetMetatable(int index)
{
	bool ret = false;
	lua_State *pLua = m_luaScript->GetLuaState();
	int topIdx = lua_gettop(pLua);

	KG_EXPECT_TRUE(pLua);
	lua_pushvalue(pLua, index);

	const luaL_Reg *luaRegTable = this->GetLuaRegTable();
	KG_EXPECT_TRUE(luaRegTable);
	lua_pushlightuserdata(pLua, (void *)luaRegTable);
	lua_gettable(pLua, LUA_REGISTRYINDEX); 

	if (!lua_istable(pLua, -1))
	{
		lua_pop(pLua, 1);
		KG_EXPECT_TRUE(this->NewMetatable());
		lua_pushlightuserdata(pLua, (void *)luaRegTable);
		lua_pushvalue(pLua, -2);
		lua_settable(pLua, LUA_REGISTRYINDEX);
	}

	lua_setmetatable(pLua, -2);
	ret = true;

exit0:
	lua_settop(pLua, topIdx);
	return ret;
}

bool LuaScriptObject::NewMetatable()
{
	bool ret = false;
	lua_State *pLua = m_luaScript->GetLuaState();
	int topIdx = lua_gettop(pLua);

	KG_EXPECT_TRUE(pLua);

	lua_newtable(pLua);
	lua_pushvalue(pLua, -1);
	lua_setfield(pLua, -2, TABLE_INDEX_FUNCTION);
	luaL_setfuncs(pLua, m_luaRegTable, 0);
	ret = true;

exit0:
	if (!ret)
		lua_settop(pLua, topIdx);

	return ret;
}

const luaL_Reg* LuaScriptObject::GetLuaRegTable()
{
	return m_luaRegTable;
}

bool LuaScriptObject::Release()
{
	this->ClearEvent();
	delete this;
	return true;
}

int LuaScriptObject::Lua_SetName(lua_State *pLua)
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

int LuaScriptObject::Lua_RegisterEvent(lua_State *pLua)
{
    const char *eventName = NULL;
    int ref = LUA_REFNIL;
    LuaScriptObject *pObject = NULL;

    KGLOG_EXPECT_TRUE(3 == lua_gettop(pLua));
    pObject = ToLuaScriptObject<LuaScriptObject>(pLua, 1);
    KGLOG_EXPECT_TRUE(pObject);

    eventName = lua_tostring(pLua, 2);
    KGLOG_EXPECT_TRUE(eventName);
    KGLOG_EXPECT_TRUE(eventName[0]);

    KGLOG_EXPECT_TRUE(lua_isfunction(pLua, 3));
	ref = luaL_ref(pLua, LUA_REGISTRYINDEX);
	KGLOG_EXPECT_TRUE(LUA_NOREF != ref && LUA_REFNIL != ref);
	KGLOG_EXPECT_TRUE(pObject->AddEvent(eventName, ref));

exit0:
    return 0;
}

int LuaScriptObject::Lua_UnregisterEvent(lua_State *pLua)
{
	const char *eventName = NULL;
	LuaScriptObject *pObject = NULL;

	KGLOG_EXPECT_TRUE(2 == lua_gettop(pLua));
	pObject = ToLuaScriptObject<LuaScriptObject>(pLua, -2);
	KGLOG_EXPECT_TRUE(pObject);

	eventName = lua_tostring(pLua, -1);
	KGLOG_EXPECT_TRUE(eventName);
	KGLOG_EXPECT_TRUE(eventName[0]);
	pObject->RemoveEvent(eventName);

exit0:
	return 0;
}

int LuaScriptObject::Lua_ClearEvent(lua_State *pLua)
{
	LuaScriptObject *pObject = nullptr;

	KGLOG_EXPECT_TRUE(1 == lua_gettop(pLua));
	pObject = ToLuaScriptObject<LuaScriptObject>(pLua, -1);
	KGLOG_EXPECT_TRUE(pObject);
	pObject->ClearEvent();

exit0:
	return 0;
}

int LuaScriptObject::Lua_Release(lua_State *pLua)
{
	LuaScriptObject **ppObject = NULL;
	LuaScriptObject *pObject = NULL;

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(1 == lua_gettop(pLua));
	KG_EXPECT_TRUE(lua_isuserdata(pLua, 1));
	pObject = ToLuaScriptObject<LuaScriptObject>(pLua, -1);
	KG_EXPECT_TRUE(pObject);
	pObject->ClearLuaUserData();
	pObject->SetWaitDispose();

exit0:
	return 0;
}

const luaL_Reg LuaScriptObject::m_luaRegTable[] = 
{
	{"SetName",         LuaScriptObject::Lua_SetName},
	{"RegisterEvent",   LuaScriptObject::Lua_RegisterEvent},
	{"UnregisterEvent", LuaScriptObject::Lua_UnregisterEvent},
	{"ClearEvent",      LuaScriptObject::Lua_ClearEvent},
	{"Dispose",         LuaScriptObject::Lua_Release},
	{"__gc",            LuaScriptObject::Lua_Release},
	{NULL,              NULL},
};