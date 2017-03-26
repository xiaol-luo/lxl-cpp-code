#ifndef __LUA_PROTOBUF_H__
#define __LUA_PROTOBUF_H__

#include <unordered_map>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>

struct lua_State;

class LuaProtobuf
{
public:
	LuaProtobuf();
	~LuaProtobuf();

	static void RegLuaProtobufFunction(lua_State *pLua);
	void AddMapPath(const char *virtualPath, const char *diskPath);
	bool Compiler(const char *filePath);


	google::protobuf::compiler::Importer * GetImporter() { return m_importer; }
	const google::protobuf::DescriptorPool *GetPool() { return m_importer->pool(); }
	google::protobuf::DynamicMessageFactory * GetMsgFactory() { return m_msgFactory; }

private:
	google::protobuf::compiler::Importer *m_importer;
	google::protobuf::compiler::DiskSourceTree *m_diskSourceTree;
	google::protobuf::compiler::MultiFileErrorCollector *m_mfec;
	google::protobuf::DynamicMessageFactory *m_msgFactory;
};

class LuaProtobufManager
{
public:
	LuaProtobufManager();
	~LuaProtobufManager();

	static LuaProtobufManager * Instance();
	
	LuaProtobuf * CreateLuaProtobuf(lua_State *pLua);
	LuaProtobuf * GetProtobuf(lua_State *pLua);
	LuaProtobuf * GetProtobuf(uint64_t key);
	void EraseLuaProtobuf(uint64_t key);
	void EraseLuaProtobuf(lua_State *pLua);

private:
	std::unordered_map<uint64_t, LuaProtobuf *> m_luaProtobufMap;
};

static int Lua_InitProtoCompiler(lua_State *pLua);
static int Lua_CreateProtoBufTable(lua_State *pLua);
static int Lua_PackupToProtoBuf(lua_State *pLua);
static int Lua_UnPackFromProtoBuf(lua_State *pLua);

#endif

