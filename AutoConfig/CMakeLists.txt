
cmake_minimum_required(VERSION 2.6)

SET(SourceFiles 
	main.cpp
	Config/ConfigBase.cpp
	Config/RankDefineConfig.cpp
	ConfigEx/ConfigExtra.cpp
	Config/ConfigUtil.cpp
)
ADD_EXECUTABLE(main ${SourceFiles})
ADD_COMPILE_OPTIONS(-g -std=c++11)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS /wd4267 /wd4244)
INCLUDE_DIRECTORIES(.)
