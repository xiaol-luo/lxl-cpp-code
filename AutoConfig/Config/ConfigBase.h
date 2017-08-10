#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"
#include "ConfigEx/ConfigExtra.h"

namespace Config
{
    class ConfigBase
    {
    public:
        int int_val;
        float float_val;
        std::vector<int> int_vec;
        std::map<int, int> int_int_map;
        std::vector<std::vector<int>> int_vec_vec;
        std::map<int, std::vector<float>> int_float_map_vec;
        ConfigEx::ConfigExtra extra_data;
        static const ConfigBase & Default();
    public:
        bool Init(std::map<std::string, std::string> kvPairs, CheckLineFunc func);
    };
}
