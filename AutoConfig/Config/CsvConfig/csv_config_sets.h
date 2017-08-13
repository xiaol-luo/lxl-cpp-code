#include <string>

namespace Config
{
    struct RankDefineSet;
    struct RankGroupSet;
    struct HeroDefineSet;
    struct ConfigBaseSet;

    struct CsvConfigSets
    {
        RankDefineSet *csv_RankDefineSet = nullptr;
        RankGroupSet *csv_RankGroupSet = nullptr;
        HeroDefineSet *csv_HeroDefineSet = nullptr;
        ConfigBaseSet *csv_ConfigBaseSet = nullptr;

        ~CsvConfigSets();
        bool Load(std::string root_path);
    };
}
