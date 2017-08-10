#include <stdio.h>
#include <vector>
#include "Config/ConfigBase.h"
#include "Config/RankDefineConfig.h"
#include "ConfigEx/ConfigExtra.h"
// #include "Config/ConfigUtil.h"

bool ConfigBaseChecLine(void* cfg)
{
    return true;
}

int main(int argc, char **argv)
{
    Config::ConfigBase cfg_base = Config::ConfigBase();
    Config::RankDefineConfig rank_cfg = Config::RankDefineConfig();
    ConfigEx::ConfigExtra().Init(cfg_base);
    ConfigEx::ConfigExtra().Init(rank_cfg);

    bool ret;
    int a;
    ret = Config::Str2BaseValue("1024", a);
    long long ll;
    ret = Config::Str2BaseValue("1024", ll);
    float f;
    ret = Config::Str2BaseValue("1024", f);
    double d;
    ret = Config::Str2BaseValue("1024", d);
    std::string str;
    ret = Config::Str2BaseValue("1024", str);

    std::vector<int> int_vec;
    ret = Config::Str2Vec("1024;1023", int_vec);
    std::map<int, std::string> int_ll_map;
    ret = Config::Str2Map("1024:abc;1023:bcd", int_ll_map);

    std::vector<std::vector<int>> int_vec_vec;
    ret = Config::Str2VecVec("1,2,3;2,2,3;3,2,3", int_vec_vec);

    std::map<int, std::vector<std::string>> int_str_map_vec;
    ret = Config::Str2MapVec("1:a,b,c;2:d,e,f;3:sdf,sfsf", int_str_map_vec);

    std::map<std::string, std::string> data;
    data[Config::ConfigBase::Default().Field_Name_int_val] = "1024";
    data[Config::ConfigBase::Default().Field_Name_float_val] = "1234";
    data[Config::ConfigBase::Default().Field_Name_int_vec] = "1;2;3;4;5";
    data[Config::ConfigBase::Default().Field_Name_int_int_map] = "1:2;2:3";
    data[Config::ConfigBase::Default().Field_Name_int_vec_vec] = "1,2,3;4,5,6";
    data[Config::ConfigBase::Default().Field_Name_int_float_map_vec] = "1:100,101,102; 2:202,203";
    ret = cfg_base.Init(data, ConfigBaseChecLine);
    printf("hello world");
}