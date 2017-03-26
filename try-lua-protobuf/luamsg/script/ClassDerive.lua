
ClassBase = {}
ClassBase.__index = ClassBase

ClassDerive = {}
ClassDerive.__index = ClassDerive

ClassFinal = {}
ClassFinal.__index = ClassFinal

setmetatable(ClassDerive, ClassBase)
setmetatable(ClassFinal, ClassDerive)


ClassBase.b = 1
ClassDerive.d = 2
ClassFinal.f = 3

