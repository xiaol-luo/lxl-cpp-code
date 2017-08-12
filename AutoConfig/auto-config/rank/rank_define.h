#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"

namespace Config
{
    struct RankDefine // 
    {
        int rankId = 0 ;
        int rankGroup = 0 ;
        std::string name;
        std::string relateParam;

        bool Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func);
    };

    struct RankDefineSet
    {
        ~RankDefineSet();
        ConfigCheckFunc cfg_check_fun = nullptr;
        ConfigSetCheckFunc cfg_set_check_fun = nullptr;
        bool Load(std::string file_path);
        
        std::vector<RankDefine *> cfg_vec;
    };
}