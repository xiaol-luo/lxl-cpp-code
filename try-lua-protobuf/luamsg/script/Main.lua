Main = {}

function Main.OnInit()
	LoadScripts("FileList.txt")
	return true
end

function Main.OnActivate()
	local a = LuaClass.New()
	local b = LuaClass.New()
	local c
	c = a + b
	c = a - b
	c = a * b
	c = a / b
	c = a % b
	c = -a
	c = a .. b
	c = #a
	c = a == b
	c = a < b
	c = a <= b
	c = a[b]
	a(b)
	print(a)
	a[b] = c

	a.TestFunc()
	a:TestFunc()

	local xxx = CreateLuaScriptObject()

	xxx:SetName("xxx")
	xxx = nil

	local demo = CreateLuaScriptDemo()
	demo:SetName("demo")

	print("0000000000000 " .. ClassFinal.b)
	demo:xxxx()

end

function Main.OnUninit()
end