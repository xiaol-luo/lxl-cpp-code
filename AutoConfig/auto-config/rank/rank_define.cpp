#include "auto-config/rank/rank_define.h"
#include "CsvParser/csv.h"

namespace Config
{
     static const char * Field_Name_rankId = "rankId";
     static const char * Field_Name_rankGroup = "rankGroup";
     static const char * Field_Name_name = "name";
     static const char * Field_Name_relateParam = "relateParam";

    bool RankDefine::Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func)
    {
        bool all_ok = true;
        all_ok = all_ok && kvPairs.count(Field_Name_rankId) > 0 && Str2BaseValue (kvPairs[Field_Name_rankId], rankId);
        all_ok = all_ok && kvPairs.count(Field_Name_rankGroup) > 0 && Str2BaseValue (kvPairs[Field_Name_rankGroup], rankGroup);
        all_ok = all_ok && kvPairs.count(Field_Name_name) > 0 && Str2Str (kvPairs[Field_Name_name], name);
        all_ok = all_ok && kvPairs.count(Field_Name_relateParam) > 0 && Str2Str (kvPairs[Field_Name_relateParam], relateParam);
        if (all_ok && nullptr != func)
            all_ok &= func(this);
        return all_ok;
    }

    bool RankDefineSet::Load(std::string file_path)
    {
        io::CSVReader<4, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'>, io::no_comment> csv_reader(file_path);
        csv_reader.read_header(io::ignore_extra_column,
            Field_Name_rankId,
            Field_Name_rankGroup,
            Field_Name_name,
            Field_Name_relateParam
            );

        std::map<std::string, std::string> kvParis;
        kvParis[Field_Name_rankId] = "";
        kvParis[Field_Name_rankGroup] = "";
        kvParis[Field_Name_name] = "";
        kvParis[Field_Name_relateParam] = "";

        bool all_ok = true;
        int curr_row = 0;
        while (csv_reader.read_row(
            kvParis[Field_Name_rankId],
            kvParis[Field_Name_rankGroup],
            kvParis[Field_Name_name],
            kvParis[Field_Name_relateParam]
            ))
        {            
            if (++ curr_row <= 1)
                continue;
            RankDefine *cfg = new RankDefine();
            all_ok &= cfg->Init(kvParis, cfg_check_fun);
            if (!all_ok)
                break;
            cfg_vec.push_back(cfg);
        }
        if (all_ok)
        {
            // gen key
            for (auto cfg : cfg_vec)
            {
            }
        }
        if (nullptr != cfg_set_check_fun)
        {
            all_ok = all_ok && cfg_set_check_fun((void *)this);
        }
        return all_ok;
    }
}
