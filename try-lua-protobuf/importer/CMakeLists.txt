cmake_minimum_required(VERSION 2.6)

include(${CMAKE_SOURCE_DIR}/common.cmake)

SET(ProjectName importer)

PROJECT(ProjectName)

SET(SourceFiles 
	main.cpp
)
source_group("" FILES ${SourceFiles})

ADD_EXECUTABLE(${ProjectName} ${SourceFiles})
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "try-probuf")
TARGET_LINK_LIBRARIES(${ProjectName} libprotobufd common libmsg)






