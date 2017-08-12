#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"

namespace Config
{
    struct RankGroup // 
    {
        int rankId = 0 ;
        int rankGroup = 0 ;
        std::string name;
        std::string relateParam;

        bool Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func);
    };

    struct RankGroupSet
    {
        ~RankGroupSet();
        ConfigCheckFunc cfg_check_fun = nullptr;
        ConfigSetCheckFunc cfg_set_check_fun = nullptr;
        bool Load(std::string file_path);
        
        std::vector<RankGroup *> cfg_vec;
    };
}