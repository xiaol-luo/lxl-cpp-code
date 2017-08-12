#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"

namespace Config
{
    struct HeroDefine // 
    {
        int prof = 0 ;
        std::string name;
        int hp = 0 ;
        int gongji = 0 ;
        int moveSpeed = 0 ;

        bool Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func);
    };

    struct HeroDefineSet
    {
        ConfigCheckFunc cfg_check_fun = nullptr;
        ConfigSetCheckFunc cfg_set_check_fun = nullptr;
        bool Load(std::string file_path);
        
        std::vector<HeroDefine *> cfg_vec;
    };
}