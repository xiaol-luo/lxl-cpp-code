#include <stdio.h>
#include <vector>
#include "Config/ConfigBase.h"
#include "Config/RankDefineConfig.h"
#include "ConfigEx/ConfigExtra.h"
// #include "Config/ConfigUtil.h"

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
    ret = Config::Str2Vector("1024;1023", int_vec);
    std::map<int, std::string> int_ll_map;
    ret = Config::Str2Map("1024:abc;1023:bcd", int_ll_map);
    printf("hello world");
}