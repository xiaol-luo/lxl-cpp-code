cmake_minimum_required(VERSION 2.6)

SET(ProjectName try-event)

PROJECT(ProjectName)

SET(SourceFiles_Main 
	main.cpp
)
source_group("" FILES ${SourceFiles_Main})

SET(SourceFiles
	${SourceFiles_Main}
)

ADD_EXECUTABLE(${ProjectName} ${SourceFiles})
SET_TARGET_PROPERTIES(${ProjectName} PROPERTIES FOLDER "examples")
TARGET_LINK_LIBRARIES(${ProjectName} event_extra event_core ws2_32 common)






