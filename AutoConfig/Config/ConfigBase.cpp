#include "Config/ConfigBase.h"
#include "CsvParser/csv.h"

namespace Config
{
    static const char * Field_Name_int_val = "int_val";
    static const char * Field_Name_float_val = "float_val";
    static const char * Field_Name_int_vec = "int_vec";
    static const char * Field_Name_int_int_map = "int_int_map";
    static const char * Field_Name_int_vec_vec = "int_vec_vec";
    static const char * Field_Name_int_float_map_vec = "int_float_map_vec";

    const ConfigBase & ConfigBase::Default()
    {
        static ConfigBase cfg;
        return cfg;
    }

    bool ConfigBase::Init(std::map<std::string, std::string> kvPairs, ConfigCheckFunc func)
    {
        bool all_ok = true;
        all_ok = all_ok && kvPairs.count(Field_Name_int_val) > 0 && Str2BaseValue(kvPairs[Field_Name_int_val], int_val);
        all_ok = all_ok && kvPairs.count(Field_Name_float_val) > 0 && Str2BaseValue(kvPairs[Field_Name_float_val], float_val);
        all_ok = all_ok && kvPairs.count(Field_Name_int_vec) > 0 && Str2Vec(kvPairs[Field_Name_int_vec], int_vec);
        all_ok = all_ok && kvPairs.count(Field_Name_int_int_map) > 0 && Str2Map(kvPairs[Field_Name_int_int_map], int_int_map);
        all_ok = all_ok && kvPairs.count(Field_Name_int_vec_vec) > 0 && Str2VecVec(kvPairs[Field_Name_int_vec_vec], int_vec_vec);
        all_ok = all_ok && kvPairs.count(Field_Name_int_float_map_vec) > 0 && Str2MapVec(kvPairs[Field_Name_int_float_map_vec], int_float_map_vec);
        all_ok = all_ok && extra_data.Init(*this);
        if (all_ok && nullptr != func)
            all_ok &= func(this);
        return all_ok;
    }

    bool ConfigBaseSet::Load(std::string file_path)
    {
        io::CSVReader<6, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'>, io::no_comment> csv_reader(file_path);
        csv_reader.read_header(io::ignore_extra_column, Field_Name_int_val, Field_Name_float_val, 
        Field_Name_int_vec, Field_Name_int_int_map, Field_Name_int_vec_vec, Field_Name_int_float_map_vec);

        std::map<std::string, std::string> kvParis;
        kvParis[Field_Name_int_val] = "";
        kvParis[Field_Name_float_val] = "";
        kvParis[Field_Name_int_vec] = "";
        kvParis[Field_Name_int_int_map] = "";
        kvParis[Field_Name_int_vec_vec] = "";
        kvParis[Field_Name_int_float_map_vec] = "";

        bool all_ok = true;
        int curr_row = 0;
        while (csv_reader.read_row(kvParis[Field_Name_int_val], kvParis[Field_Name_float_val],  
            kvParis[Field_Name_int_vec], kvParis[Field_Name_int_int_map],
            kvParis[Field_Name_int_vec_vec], kvParis[Field_Name_int_float_map_vec]))
        {            
            if (++ curr_row <= 1)
                continue;
            ConfigBase cfg;
            all_ok &= cfg.Init(kvParis, cfg_check_fun);
            if (!all_ok)
                break;
            cfg_vec.push_back(cfg);
        }
        return all_ok;
    }
}



