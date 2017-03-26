
LuaClass = {}
local mt = {}

function LuaClass.New(t)
	local _t = t
	if not _t then 
		_t = {}
	end
	
	setmetatable(_t, mt)
	return _t
end

mt.__add = function (a, b)
	print("a + b -> __add")
end

mt.__sub = function (a, b)
	print("a - b -> __sub")
end

mt.__mul = function (a, b)
	print("a * b -> __mul")
end

mt.__div = function (a, b)
	print("a / b -> __div")
end

mt.__mod = function (a, b)
	print("a % b -> __mod")
end

mt.__pow = function (a, b)
	print("pow(a, b) -> __pow")
end

mt.__unm = function (a)
	print("-a -> __unm")
end

mt.__concat = function (a, b)
	print("a .. b -> __concat")
end

mt.__len = function (a)
	print("#a -> __len")
end

mt.__eq = function (a, b)
	print("a == b -> __eq")
end

mt.__lt = function (a, b)
	print("a < b -> __lt")
end

mt.__le = function (a, b)
	print("a <= b -> __le")
end

mt.__index = function (a, b)
	print("a[b] -> __index")
	return mt[b]
end
	
mt.__newindex = function (a, b, c)
	print("a[b] = c -> __newindex")
end

mt.__call = function (a, ...)
	print("a(...) -> __call")
end

mt.__tostring = function (a)
	return "a.tostring -> __tostring"
end

mt.TestFunc = function ()
	print("excute LuaClass::TestFunc")
end

--[[
__add(a, b) --加法
__sub(a, b) --减法
__mul(a, b) --乘法
__div(a, b) --除法
__mod(a, b) --取模
__pow(a, b) --乘幂
__unm(a) --相反数
__concat(a, b) --连接
__len(a) --长度
__eq(a, b) --相等
__lt(a, b) --小于
__le(a, b) --小于等于
__index(a, b) --索引查询
__newindex(a, b, c) --索引更新（PS：不懂的话，后面会有讲）
__call(a, ...) --执行方法调用
__tostring(a) --字符串输出
__metatable --保护元表
]]