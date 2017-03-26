#include "LuaProtobuf.h"
#include "lua.hpp"
#include "lua.h"
#include "lauxlib.h"
#include "Common.h"

extern const luaL_Reg local_LuaProtobufScriptTable[];


class DefaultMultiFileErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
	DefaultMultiFileErrorCollector() : google::protobuf::compiler::MultiFileErrorCollector() {}
	virtual ~DefaultMultiFileErrorCollector() {}

	virtual void AddError(const std::string& filename, int line, int column,
		const std::string& message) {}
};

LuaProtobuf::LuaProtobuf() : m_importer(nullptr), m_diskSourceTree(nullptr), m_mfec(nullptr), m_msgFactory(nullptr)
{
	m_diskSourceTree = new google::protobuf::compiler::DiskSourceTree();
	m_mfec = new DefaultMultiFileErrorCollector();
	m_importer = new google::protobuf::compiler::Importer(m_diskSourceTree, m_mfec);
	m_msgFactory = new google::protobuf::DynamicMessageFactory(m_importer->pool());
}

LuaProtobuf::~LuaProtobuf()
{
	KG_DELETE(m_msgFactory);
	KG_DELETE(m_importer);
	KG_DELETE(m_mfec);
	KG_DELETE(m_diskSourceTree);
}

void LuaProtobuf::RegLuaProtobufFunction(lua_State *pLua)
{
	if (nullptr == pLua)
		return;

	int topIdx = lua_gettop(pLua);
	lua_pushglobaltable(pLua);
	luaL_setfuncs(pLua, local_LuaProtobufScriptTable, 0);
	lua_settop(pLua, topIdx);
}

void LuaProtobuf::AddMapPath(const char *virtualPath, const char *diskPath)
{
	m_diskSourceTree->MapPath(virtualPath, diskPath);
}

bool LuaProtobuf::Compiler(const char *filePath)
{
	return nullptr != m_importer->Import(filePath);
}

LuaProtobufManager::LuaProtobufManager()
{

}

LuaProtobufManager::~LuaProtobufManager()
{
	for (auto it = m_luaProtobufMap.begin(); m_luaProtobufMap.end() != it; ++ it)
	{
		delete it->second;
	}
	m_luaProtobufMap.clear();
}

LuaProtobufManager * LuaProtobufManager::Instance()
{
	static LuaProtobufManager instance;
	return &instance;
}

LuaProtobuf * LuaProtobufManager::CreateLuaProtobuf(lua_State *pLua)
{
	LuaProtobuf *pLuaProtobuf = this->GetProtobuf(pLua);
	if (nullptr == pLuaProtobuf)
	{
		uint64_t key = (int64_t)pLua;
		pLuaProtobuf = new LuaProtobuf();
		m_luaProtobufMap[key] = pLuaProtobuf;
	}

	return pLuaProtobuf;
}

LuaProtobuf * LuaProtobufManager::GetProtobuf(lua_State *pLua)
{
	uint64_t key = (int64_t)pLua;
	return this->GetProtobuf(key);
}

LuaProtobuf * LuaProtobufManager::GetProtobuf(uint64_t key)
{
	auto it = m_luaProtobufMap.find(key);
	if (m_luaProtobufMap.end() != it)
		return it->second;
	return nullptr;
}

void LuaProtobufManager::EraseLuaProtobuf(uint64_t key)
{
	LuaProtobuf *pLuaProtobuf = this->GetProtobuf(key);
	if (nullptr != pLuaProtobuf)
	{
		m_luaProtobufMap.erase(key);
		KG_DELETE(pLuaProtobuf);
	}
}

void LuaProtobufManager::EraseLuaProtobuf(lua_State *pLua)
{
	int64_t key = (int64_t)pLua;
	this->EraseLuaProtobuf(key);
}


int Lua_InitProtoCompiler(lua_State *pLua)
{
	bool ret = false;
	int paramCount = lua_gettop(pLua);
	KGLOG_EXPECT_TRUE(1 == paramCount);
	KGLOG_EXPECT_TRUE(lua_isstring(pLua, -1));

	const char *filePath = lua_tostring(pLua, -1);
	LuaProtobuf *pProtobuf = LuaProtobufManager::Instance()->GetProtobuf(pLua);
	KGLOG_EXPECT_TRUE(pProtobuf);
	ret = pProtobuf->Compiler(filePath);

exit0:
	lua_pushboolean(pLua, ret);
	return 1;
}

extern google::protobuf::Message * LuaTable2ProtobufBinary(lua_State *pLua, google::protobuf::DynamicMessageFactory *msgFactory,
	const google::protobuf::DescriptorPool *pool, const google::protobuf::Descriptor *desc);
extern bool ProtobufBinary2LuaTable(lua_State *pLua, const google::protobuf::Message *msg, google::protobuf::DynamicMessageFactory *msgFactory,
	const google::protobuf::DescriptorPool *pool, const google::protobuf::Descriptor *desc);

int Lua_CreateProtoBufTable(lua_State *pLua)
{
	bool ret = false;
	int paramCount = lua_gettop(pLua);
	KGLOG_EXPECT_TRUE(1 == paramCount);
	KGLOG_EXPECT_TRUE(lua_isstring(pLua, -1));
	const char *msgName = lua_tostring(pLua, -1);
	
	LuaProtobuf *pProtobuf = LuaProtobufManager::Instance()->GetProtobuf(pLua);
	KGLOG_EXPECT_TRUE(pProtobuf);
	google::protobuf::compiler::Importer *pImporter = pProtobuf->GetImporter();
	google::protobuf::DynamicMessageFactory *msgFactory = pProtobuf->GetMsgFactory();
	const google::protobuf::DescriptorPool *pool = pProtobuf->GetPool();
	const google::protobuf::Descriptor *desc = pool->FindMessageTypeByName(msgName);
	KGLOG_EXPECT_TRUE(desc);
	const google::protobuf::Message *msg = msgFactory->GetPrototype(desc);
	KGLOG_EXPECT_TRUE(msg);
	const google::protobuf::Message::Reflection *refection = msg->GetReflection();
	KGLOG_EXPECT_TRUE(refection);

	lua_newtable(pLua);
	ret = ProtobufBinary2LuaTable(pLua, msg, msgFactory, pool, desc);

	/*
	for (int i = 0; i < desc->field_count(); ++i)
	{
		int topidx = lua_gettop(pLua);
		const google::protobuf::FieldDescriptor *fieldDesc = desc->field(i);
		const std::string &fieldName = fieldDesc->name();

		switch (fieldDesc->type())
		{
			case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				break;

			case google::protobuf::FieldDescriptor::TYPE_INT32:
				int val = refection->GetInt32(*msg, fieldDesc);
				lua_pushinteger(pLua, val);
				lua_setfield(pLua, -2, fieldName.c_str());
				break;
		}

		lua_settop(pLua, topidx);
	}
	*/
	
exit0:
	return ret ? 1 : 0;
}

typedef size_t KeyValLenType;


int Lua_PackKeyValueToBuffer(lua_State *pLua)
{
	int nResult = 0;
	int topIdx = lua_gettop(pLua);
	const static int BUFF_SIZE = 10240;
	char buff[BUFF_SIZE] = { 0 };
	char *pCurrBuf = buff;
	const char *pKey = nullptr;
	const char *pVal = nullptr;
	KeyValLenType keyLen = 0;
	KeyValLenType valLen = 0;

	KGLOG_EXPECT_TRUE(1 == topIdx);
	KGLOG_EXPECT_TRUE(lua_istable(pLua, -1));

	lua_pushnil(pLua);
	while (lua_next(pLua, -2))
	{
		KGLOG_EXPECT_TRUE(lua_isstring(pLua, -1));
		KGLOG_EXPECT_TRUE(lua_isstring(pLua, -2));
		pKey = lua_tolstring(pLua, -2, &keyLen);
		pVal = lua_tolstring(pLua, -1, &valLen);
		lua_pop(pLua, 1);

		const static int LEN_PARAM_SIZE = sizeof(KeyValLenType);
		KG_EXPECT_TRUE(LEN_PARAM_SIZE  * 2 + (int)keyLen + (int)valLen <= buff + BUFF_SIZE - pCurrBuf);
		
		*(KeyValLenType *)pCurrBuf = keyLen; pCurrBuf += LEN_PARAM_SIZE;
		memcpy(pCurrBuf, pKey, keyLen); pCurrBuf += keyLen;
		*(KeyValLenType *)pCurrBuf = valLen; pCurrBuf += LEN_PARAM_SIZE;
		memcpy(pCurrBuf, pVal, valLen); pCurrBuf += valLen;
	}

	lua_pushlstring(pLua, buff, pCurrBuf - buff);
	++nResult;

exit0:
	return nResult;

}

int Lua_UnpackKeyValueBuffer(lua_State *pLua)
{
	int nResult = 0;
	int topIdx = lua_gettop(pLua);
	const char *buff = nullptr;
	size_t buffLen = 0;
	const char *pCurrBuff = nullptr;
	const char *pBuffEnd = nullptr;
	int leftBuffLen = 0;
	KeyValLenType keyLen = 0;
	KeyValLenType valLen = 0;
	const char *pKey = nullptr;
	const char *pVal = nullptr;

	KG_EXPECT_TRUE(1 == topIdx);
	KG_EXPECT_TRUE(lua_isstring(pLua, -1));

	buff = lua_tolstring(pLua, -1, &buffLen);
	KG_EXPECT_TRUE(buff && buffLen > 0);

	lua_newtable(pLua);
	leftBuffLen = (int)buffLen;
	pCurrBuff = buff;
	pBuffEnd = buff + buffLen;

	while (pCurrBuff < pBuffEnd)
	{
		const static int LEN_PARAM_SIZE = sizeof(KeyValLenType);

		KG_EXPECT_TRUE(pBuffEnd - pCurrBuff >= LEN_PARAM_SIZE);
		keyLen = *(KeyValLenType *)pCurrBuff; pCurrBuff += LEN_PARAM_SIZE;
		KG_EXPECT_TRUE(keyLen > 0);
		KG_EXPECT_TRUE(pBuffEnd - pCurrBuff >= (int)keyLen);
		pKey = pCurrBuff; pCurrBuff += keyLen;

		KG_EXPECT_TRUE(pBuffEnd - pCurrBuff >= LEN_PARAM_SIZE);
		valLen = *(KeyValLenType *)pCurrBuff; pCurrBuff += LEN_PARAM_SIZE;
		KG_EXPECT_TRUE(valLen > 0);
		KG_EXPECT_TRUE(pBuffEnd - pCurrBuff >= (int)valLen);
		pVal = pCurrBuff; pCurrBuff += valLen;

		lua_pushlstring(pLua, pKey, keyLen);
		lua_pushlstring(pLua, pVal, valLen);
		lua_settable(pLua, -3);
	}

	++nResult;

exit0:
	return nResult;
}

int Lua_PackupToProtoBuf(lua_State *pLua)
{
	bool ret = false;
	google::protobuf::Message *msg = nullptr;
	std::string binaryData;

	int paramCount = lua_gettop(pLua);
	KGLOG_EXPECT_TRUE(2 == paramCount);
	KGLOG_EXPECT_TRUE(lua_isstring(pLua, -2));
	KGLOG_EXPECT_TRUE(lua_istable(pLua, -1));
	LuaProtobuf *pProtobuf = LuaProtobufManager::Instance()->GetProtobuf(pLua);
	KGLOG_EXPECT_TRUE(pProtobuf);
	google::protobuf::compiler::Importer *pImporter = pProtobuf->GetImporter();
	google::protobuf::DynamicMessageFactory *msgFactory = pProtobuf->GetMsgFactory();
	const google::protobuf::DescriptorPool *pool = pProtobuf->GetPool();
	const char *msgName = lua_tostring(pLua, -2);
	const google::protobuf::Descriptor *desc = pool->FindMessageTypeByName(msgName);
	KGLOG_EXPECT_TRUE(desc);

	msg = LuaTable2ProtobufBinary(pLua, msgFactory, pool, desc);
	KGLOG_EXPECT_TRUE(msg);
	msg->SerializeToString(&binaryData);
	size_t binaryLen = binaryData.size();
	lua_pushlstring(pLua, binaryData.data(), binaryLen);

	ret = true;

exit0:	
	KG_DELETE(msg);
	return ret ? 1 : 0;
}

int Lua_UnPackFromProtoBuf(lua_State *pLua)
{
	bool ret = false;
	google::protobuf::Message *msg = nullptr;

	int paramCount = lua_gettop(pLua);
	KGLOG_EXPECT_TRUE(2 == paramCount);
	KGLOG_EXPECT_TRUE(lua_isstring(pLua, -2));
	KGLOG_EXPECT_TRUE(lua_isstring(pLua, -1));
	const char *msgName = lua_tostring(pLua, -2);
	size_t binaryLen = 0;
	const char *binaryData = lua_tolstring(pLua, -1, &binaryLen);

	LuaProtobuf *pProtobuf = LuaProtobufManager::Instance()->GetProtobuf(pLua);
	KGLOG_EXPECT_TRUE(pProtobuf);
	google::protobuf::compiler::Importer *pImporter = pProtobuf->GetImporter();
	google::protobuf::DynamicMessageFactory *msgFactory = pProtobuf->GetMsgFactory();
	const google::protobuf::DescriptorPool *pool = pProtobuf->GetPool();
	const google::protobuf::Descriptor *desc = pool->FindMessageTypeByName(msgName);
	KGLOG_EXPECT_TRUE(desc);
	const google::protobuf::Message *msgPrototype = msgFactory->GetPrototype(desc);
	KGLOG_EXPECT_TRUE(msgPrototype);
	msg = msgPrototype->New();
	KGLOG_EXPECT_TRUE(msg);
	KGLOG_EXPECT_TRUE(msg->ParseFromArray(binaryData, binaryLen));

	lua_newtable(pLua);
	ret = ProtobufBinary2LuaTable(pLua, msg, msgFactory, pool, desc);

exit0:
	KG_DELETE(msg);
	return ret ? 1 : 0;
}

luaL_Reg const static local_LuaProtobufScriptTable[] =
{
	{ "InitProtoCompiler", Lua_InitProtoCompiler },
	{ "PackupToProtoBuf", Lua_PackupToProtoBuf },
	{ "UnPackFromProtoBuf", Lua_UnPackFromProtoBuf },
	{ "CreateProtoBufTable", Lua_CreateProtoBufTable },
	{ "UnpackKeyValueBuffer", Lua_UnpackKeyValueBuffer },
	{ "PackKeyValueToBuffer", Lua_PackKeyValueToBuffer },
	{ NULL, NULL },
};

google::protobuf::Message * LuaTable2ProtobufBinary(lua_State *pLua, google::protobuf::DynamicMessageFactory *msgFactory,
	const google::protobuf::DescriptorPool *pool, const google::protobuf::Descriptor *desc)
{
	bool ret = false;
	google::protobuf::Message *msg = nullptr;
	google::protobuf::Message *fieldMsg = nullptr;

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(msgFactory);
	KG_EXPECT_TRUE(pool);
	KG_EXPECT_TRUE(desc);
	KG_EXPECT_TRUE(lua_gettop(pLua) > 0);
	KG_EXPECT_TRUE(lua_istable(pLua, -1));

	const google::protobuf::Message *msgPrototype = msgFactory->GetPrototype(desc);
	KGLOG_EXPECT_TRUE(msgPrototype);
	const google::protobuf::Message::Reflection *refection = msgPrototype->GetReflection();
	KGLOG_EXPECT_TRUE(refection);
	msg = msgPrototype->New();
	KGLOG_EXPECT_TRUE(msg);

	const int tableIdx = lua_gettop(pLua);

	for (int i = 0; i < desc->field_count(); ++i)
	{
		int topidx = lua_gettop(pLua);
		const google::protobuf::FieldDescriptor *fieldDesc = desc->field(i);
		const std::string &fieldName = fieldDesc->name();

		lua_getfield(pLua, -1, fieldName.c_str());
		bool isRepeated = fieldDesc->is_repeated();
		if (isRepeated)
			KG_EXPECT_TRUE(lua_istable(pLua, -1));

		switch (fieldDesc->type())
		{
			case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				{
					KGLOG_EXPECT_TRUE(lua_istable(pLua, -1));
					fieldMsg = LuaTable2ProtobufBinary(pLua, msgFactory, pool, fieldDesc->message_type());
					KGLOG_EXPECT_TRUE(fieldMsg);
					refection->SetAllocatedMessage(msg, fieldMsg, fieldDesc);
					fieldMsg = nullptr;
				}
				break;

			case google::protobuf::FieldDescriptor::TYPE_INT32:
				{
					if (!isRepeated)
					{
						int val = (int)lua_tointeger(pLua, -1);
						refection->SetInt32(msg, fieldDesc, val);
					}
					else
					{
						int fieldIdx = 0;
						lua_pushnil(pLua);
						while (lua_next(pLua, -2))
						{
							if (lua_isinteger(pLua, -1) && lua_isinteger(pLua, -2))
							{
								int idx = (int)lua_tointeger(pLua, -2);
								int val = (int)lua_tointeger(pLua, -1);

								refection->AddInt32(msg, fieldDesc, val);
							}

							lua_pop(pLua, 1);
						}
					}
				}
				break;

			case google::protobuf::FieldDescriptor::TYPE_STRING:
				{
					size_t strLen = 0;
					const char *str = lua_tolstring(pLua, -1, &strLen);
					std::string stdStr(str, strLen);
					refection->SetString(msg, fieldDesc, stdStr);
				}
				break;
		}

		lua_settop(pLua, topidx);
	}

	ret = true;

exit0:
	if (!ret)
	{
		KG_DELETE(fieldMsg);
		KG_DELETE(msg);
	}

	return msg;
}

bool ProtobufBinary2LuaTable(lua_State *pLua, const google::protobuf::Message *msg, google::protobuf::DynamicMessageFactory *msgFactory,
	const google::protobuf::DescriptorPool *pool, const google::protobuf::Descriptor *desc)
{
	bool ret = false;
	int topIdx = lua_gettop(pLua);

	KG_EXPECT_TRUE(pLua);
	KG_EXPECT_TRUE(msg);
	KG_EXPECT_TRUE(msgFactory);
	KG_EXPECT_TRUE(pool);
	KG_EXPECT_TRUE(desc);
	KG_EXPECT_TRUE(lua_gettop(pLua) > 0);
	KG_EXPECT_TRUE(lua_istable(pLua, -1));

	const google::protobuf::Message::Reflection *refection = msg->GetReflection();
	KGLOG_EXPECT_TRUE(refection);

	for (int i = 0; i < desc->field_count(); ++i)
	{
		int top = lua_gettop(pLua);
		const google::protobuf::FieldDescriptor *fieldDesc = desc->field(i);
		const std::string &fieldName = fieldDesc->name();

		switch (fieldDesc->type())
		{
			case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				{
					const google::protobuf::Message &fieldMsg = refection->GetMessage(*msg, fieldDesc, msgFactory);
					lua_newtable(pLua);
					bool succ = ProtobufBinary2LuaTable(pLua, &fieldMsg, msgFactory, pool, fieldDesc->message_type());
					KGLOG_EXPECT_TRUE(succ);
					lua_setfield(pLua, -2, fieldName.c_str());
				}
				break;

			case google::protobuf::FieldDescriptor::TYPE_INT32:
				{
					if (fieldDesc->is_repeated())
					{
						lua_newtable(pLua);
						//lua_setfield(pLua, -1, fieldName.c_str());
						//lua_getfield(pLua, -1, fieldName.c_str());
						int fieldCount = refection->FieldSize(*msg, fieldDesc);
						for (int i = 0; i < fieldCount; ++ i)
						{
							int val = refection->GetRepeatedInt32(*msg, fieldDesc, i);
							lua_pushinteger(pLua, i);
							lua_pushinteger(pLua, val);
							lua_settable(pLua, -3);
						}
						lua_setfield(pLua, -2, fieldName.c_str());
					}
					else
					{
						int val = refection->GetInt32(*msg, fieldDesc);
						lua_pushinteger(pLua, val);
						lua_setfield(pLua, -2, fieldName.c_str());
					}
				}
				break;
			case google::protobuf::FieldDescriptor::TYPE_STRING:
				{
					std::string str = refection->GetString(*msg, fieldDesc);
					lua_pushlstring(pLua, str.data(), str.size());
					lua_setfield(pLua, -2, fieldName.c_str());
				}
				break;
		}

		lua_settop(pLua, top);
	}

	ret = true;

exit0:
	lua_settop(pLua, topIdx);
	return ret;
}
