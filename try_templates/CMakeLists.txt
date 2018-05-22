CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

include (Utils.cmake)

SET(ProjectName try_templates)
PROJECT(${ProjectName})

SET(Cpp_File_Pattern "*.hpp" "*.cpp" "*.h" "*.c" "*.cc")
FileGlobRecurse(${CMAKE_CURRENT_SOURCE_DIR} "${Cpp_File_Pattern}" dir_files)
SET(SourceFiles ${SourceFiles} ${dir_files})
FileGroupByDir(${CMAKE_CURRENT_SOURCE_DIR} "${dir_files}" "")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

IF (WIN32)
	ADD_DEFINITIONS(/D NOMINMAX)
	ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS /wd4267 /wd4244 /wd4996 /wd4800 /D _WINSOCK_DEPRECATED_NO_WARNINGS)
ELSE ()
	ADD_COMPILE_OPTIONS(-g)
ENDIF (WIN32)

ADD_COMPILE_OPTIONS(-std=c++11)

ADD_EXECUTABLE(${ProjectName} ${SourceFiles})


