
cmake_minimum_required(VERSION 2.6)

SET(SourceFiles 
	main.cpp
	ConfigEx/ConfigExtra.cpp
	Config/ConfigUtil.cpp
	csv_config_sets.cpp
	auto-config/config_base.cpp
	auto-config/hero/hero_define.cpp
	auto-config/rank/rank_define.cpp
	auto-config/rank/rank_group.cpp
)
ADD_EXECUTABLE(main ${SourceFiles})
ADD_COMPILE_OPTIONS(-g -std=c++11)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS /wd4267 /wd4244)
INCLUDE_DIRECTORIES(.)
