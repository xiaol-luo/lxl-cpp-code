#include "Config/ConfigBase.h"

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

    bool ConfigBase::Init(std::map<std::string, std::string> kvPairs, CheckLineFunc func)
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
}



