#pragma once
#include <string>
#include <vector>
#include <map>
#include "Config/ConfigUtil.h"
#include "ConfigEx/ConfigExtra.h"

namespace Config
{
    class ConfigBase
    {
    private:
        int m_int_val;
        float m_float_val;
        std::vector<int> m_int_vec;
        std::map<int, int> m_int_int_map;
        std::vector<std::vector<int>> m_int_vec_vec;
        std::map<int, std::vector<float>> m_int_float_map_vec;
        ConfigEx::ConfigExtra m_extra_data;
    public:
        int int_val() const { return m_int_val; }
        float float_val() const { return m_float_val; }
        const std::vector<int> & int_vec() const { return m_int_vec; }
        const std::map<int, int> & int_int_map() const { return m_int_int_map; }
        const std::vector<std::vector<int>> & int_vec_vec() const { return m_int_vec_vec; }
        const std::map<int, std::vector<float>> & int_float_map_vec() const { return m_int_float_map_vec; }
        const ConfigEx::ConfigExtra & extra_data() const { return m_extra_data; }
        static const ConfigBase & Default();
    public:
        const char * Field_Name_int_val = "int_val";
        const char * Field_Name_float_val = "float_val";
        const char * Field_Name_int_vec = "int_vec";
        const char * Field_Name_int_int_map = "int_int_map";
        const char * Field_Name_int_vec_vec = "int_vec_vec";
        const char * Field_Name_int_float_map_vec = "int_float_map_vec";

        bool Init(std::map<std::string, std::string> kvPairs, CheckLineFunc func)
        {
            bool all_ok = true;
            all_ok = all_ok && kvPairs.count(Field_Name_int_val) > 0 && Str2BaseValue(kvPairs[Field_Name_int_val], m_int_val);
            all_ok = all_ok && kvPairs.count(Field_Name_float_val) > 0 && Str2BaseValue(kvPairs[Field_Name_float_val], m_float_val);
            all_ok = all_ok && kvPairs.count(Field_Name_int_vec) > 0 && Str2Vec(kvPairs[Field_Name_int_vec], m_int_vec);
            all_ok = all_ok && kvPairs.count(Field_Name_int_int_map) > 0 && Str2Map(kvPairs[Field_Name_int_int_map], m_int_int_map);
            all_ok = all_ok && kvPairs.count(Field_Name_int_vec_vec) > 0 && Str2VecVec(kvPairs[Field_Name_int_vec_vec] ,m_int_vec_vec);
            all_ok = all_ok && kvPairs.count(Field_Name_int_float_map_vec) > 0 && Str2MapVec(kvPairs[Field_Name_int_float_map_vec], m_int_float_map_vec);
            all_ok = all_ok && m_extra_data.Init(*this);
            if (all_ok && nullptr != func)
                all_ok &= func(this);
            return all_ok;
        }
    };
}
