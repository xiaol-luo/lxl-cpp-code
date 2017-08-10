#include <stdio.h>
#include <vector>
#include "Config/ConfigBase.h"
#include "Config/RankDefineConfig.h"
#include "ConfigEx/ConfigExtra.h"
#include "Config/ConfigUtil.h"

int main(int argc, char **argv)
{
    Config::ConfigBase cfg_base = Config::ConfigBase();
    int a = cfg_base.GetField<int>("xxx");

    RankDefineConfig rank_cfg = RankDefineConfig();
    rank_cfg.GetField<float>("222");
    rank_cfg.GetField<int>("222");

    ConfigEx::ConfigExtra cfg_extra = ConfigEx::ConfigExtra();
    cfg_extra.Init(cfg_base);

    bool ret;
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