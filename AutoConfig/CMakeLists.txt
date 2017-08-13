
cmake_minimum_required(VERSION 2.6)

SET(SourceFiles_3rdPartLibs
	3rdPartLibs/IniFile/IniFile.cpp
)

SET(SourceFiles_OwnLibs
	OwnLibs/Utils/ConfigUtil.cpp
)

SET(SourceFiles 
	main.cpp
	Config/ConfigEx/ConfigExtra.cpp
	Config/CsvConfig/csv_config_sets.cpp
	Config/CsvConfig/config_base/config_base.cpp
	Config/CsvConfig/hero/hero_define.cpp
	Config/CsvConfig/rank/rank_define.cpp
	Config/CsvConfig/rank/rank_group.cpp
)

ADD_EXECUTABLE(main ${SourceFiles} ${SourceFiles_3rdPartLibs} ${SourceFiles_OwnLibs})
ADD_COMPILE_OPTIONS(-g -std=c++11)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS /wd4267 /wd4244)
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(./3rdPartLibs)
INCLUDE_DIRECTORIES(./OwnLibs)
INCLUDE_DIRECTORIES(./Config)
INCLUDE_DIRECTORIES(./Config/CsvConfig)
