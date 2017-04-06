cmake_minimum_required(VERSION 2.6)

if(WIN32)
  set_property(GLOBAL PROPERTY USE_FOLDERS On)
endif(WIN32)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

Set(ProjectName common)
PROJECT(${ProjectName})

SET(SourceFiles_Log 
	Log/Log.h
	Log/Log.cpp
)
source_group("Log" FILES ${SourceFiles_Log})

SET(SourceFiles
	${SourceFiles_Log}
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdpart)

ADD_LIBRARY(${ProjectName} ${SourceFiles})
