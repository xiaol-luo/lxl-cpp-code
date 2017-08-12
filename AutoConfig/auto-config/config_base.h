#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"
#include "ConfigEx/ConfigExtra.h"

namespace Config
{
    struct ConfigBase // 
    {
        int int_val = 0 ;
        float float_val = 0 ;
        std::vector<int> int_vec;
        std::map<int, int> int_int_map;
        std::vector<std::vector<int>> int_vec_vec;
        std::map<int, std::vector<float>> int_float_map_vec;
        std::string str_val;
        ConfigEx::ConfigExtra extra_data;

        bool Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func);
    };

    struct ConfigBaseSet
    {
        ~ConfigBaseSet();
        ConfigCheckFunc cfg_check_fun = nullptr;
        ConfigSetCheckFunc cfg_set_check_fun = nullptr;
        bool Load(std::string file_path);
        
        std::vector<ConfigBase *> cfg_vec;
        std::map<int, ConfigBase *> int_val_to_key;
        std::map<float, std::vector<ConfigBase *>> float_val_to_group;
        std::map<std::string, ConfigBase *> str_val_to_key;
        std::map<std::string, std::vector<ConfigBase *>> str_val_to_group;
    };
}