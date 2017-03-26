cmake_minimum_required(VERSION 2.6)

include(${CMAKE_SOURCE_DIR}/common.cmake)

set(Files_root
	GlobalDef.h
	Common.h
	KGPublic.h
)
source_group("" FILES ${Files_root})

set(Files_compat
	Compat/Compat.h
	Compat/LinuxCompat.cc
	Compat/WinCompat.cc
)
source_group("Compat" FILES ${Files_compat})

set(Files_Util
	Util/Util.h
	Util/Util.cpp
)
source_group("Util" FILES ${Files_Util})

set(Files_Log
	Log/Log.h
	Log/Log.cpp
)
source_group("Log" FILES ${Files_Log})

set(Files_
	${Files_root}
	${Files_compat}
	${Files_Util}
	${Files_Log}
)

set(ProjectName common)
Add_LIBRARY(${ProjectName} STATIC ${Files_})
