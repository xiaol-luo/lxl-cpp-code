#include "csv_config_sets.h"
#include "rank/rank_define.h"
#include "rank/rank_group.h"
#include "hero/hero_define.h"
#include "config_base/config_base.h"

namespace Config
{
    CsvConfigSets::~CsvConfigSets()
    {
        delete csv_RankDefineSet;
        delete csv_RankGroupSet;
        delete csv_HeroDefineSet;
        delete csv_ConfigBaseSet;
    }

    bool CsvConfigSets::Load(std::string root_path)
    {
        while (!root_path.empty())
        {
            if (root_path.back() != '\\' && root_path.back() != '/' && root_path.back() != ' ')
                break;
            root_path.pop_back();
        }
        csv_RankDefineSet = new RankDefineSet;
        csv_RankGroupSet = new RankGroupSet;
        csv_HeroDefineSet = new HeroDefineSet;
        csv_ConfigBaseSet = new ConfigBaseSet;

        bool all_ok = true;
        if (all_ok)
        {
            all_ok = csv_RankDefineSet->Load(root_path + '/' + "rank/rank_define.csv");
        }
        if (all_ok)
        {
            all_ok = csv_RankGroupSet->Load(root_path + '/' + "rank/rank_group.csv");
        }
        if (all_ok)
        {
            all_ok = csv_HeroDefineSet->Load(root_path + '/' + "hero/hero_define.csv");
        }
        if (all_ok)
        {
            all_ok = csv_ConfigBaseSet->Load(root_path + '/' + "config_base/config_base.csv");
        }

        if (!all_ok)
        {
            delete csv_RankDefineSet; csv_RankDefineSet = nullptr;
            delete csv_RankGroupSet; csv_RankGroupSet = nullptr;
            delete csv_HeroDefineSet; csv_HeroDefineSet = nullptr;
            delete csv_ConfigBaseSet; csv_ConfigBaseSet = nullptr;
        }

        return all_ok;
    }
}