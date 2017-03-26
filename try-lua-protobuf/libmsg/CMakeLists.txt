cmake_minimum_required(VERSION 2.6)

include(${CMAKE_SOURCE_DIR}/common.cmake)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/protobuf/libs)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/protobuf/include)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/lua5.3/libs)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/lua5.3/include)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/libmsg)

SET(ProjectName libmsg)

PROJECT(ProjectName)

SET(SourceFiles 
	code.pb.cc
	code.pb.h
)
source_group("" FILES ${SourceFiles})

add_library(${ProjectName} ${SourceFiles})
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "try-probuf")

#TARGET_LINK_LIBRARIES(${ProjectName} libprotobufd common)






