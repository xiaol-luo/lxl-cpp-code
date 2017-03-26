LuaMsg = {}

function LuaMsg.OnInit()
	if LoadScripts("FileList.txt") then 
		print("LuaMsg.OnInit success")
		return "12345"
	end

	return false
end

function LuaMsg.OnActivate()
	KLog.Debug("-- LuaMsg.OnActivate");
	local ret = InitProtoCompiler("proto/code.proto");

	local protoName = "Code"
	local t1 = CreateProtoBufTable(protoName)

	print("++++++++ create t1 +++++++++")
	PrintTable(t1)

	t1.msgID = 1
	t1.name = "outer"
	t1.ping.msgID = 20
	t1.ping.userid = 30
	t1.ping.username= "inner"
	table.insert(t1.xx, 1)
	table.insert(t1.xx, 2)

	local binary = PackupToProtoBuf(protoName, t1)
	t2 = UnPackFromProtoBuf(protoName, binary)

	print("++++++++ t2 +++++++++")
	PrintTable(t2)

end

function LuaMsg.OnUninit()
end

