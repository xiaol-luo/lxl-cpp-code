cmake_minimum_required(VERSION 2.6)

include(${CMAKE_SOURCE_DIR}/common.cmake)

SET(ProjectName luamsg)

PROJECT(ProjectName)

SET(SourceFiles_Main 
	main.cpp
	LuaProtobuf.cpp
	LuaProtobuf.h
	LuaUtil.h
	LuaUtil.cpp
	LuaScript.h
	LuaScript.cpp
)
source_group("" FILES ${SourceFiles_Main})

SET(SourceFiles_ScriptObject
	ScriptObject/LuaScriptObject.h
	ScriptObject/LuaScriptObject.cpp
)
source_group("ScriptObject" FILES ${SourceFiles_ScriptObject})

SET(SourceFiles_ScriptObject_Config
	ScriptObject/Config/LuaCsvFile.h
	ScriptObject/Config/LuaCsvFile.cpp
	ScriptObject/Config/LuaIniFile.h
	ScriptObject/Config/LuaIniFile.cpp
	ScriptObject/LuaScriptDemo.h
	ScriptObject/LuaScriptDemo.cpp
)
source_group("ScriptObject" FILES ${SourceFiles_ScriptObject_Config})

SET(SourceFiles
	${SourceFiles_Main}
	${SourceFiles_ScriptObject}	
	${SourceFiles_ScriptObject_Config}
)

ADD_EXECUTABLE(${ProjectName} ${SourceFiles})
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "try-probuf")
TARGET_LINK_LIBRARIES(${ProjectName} libprotobufd common libmsg liblua)






