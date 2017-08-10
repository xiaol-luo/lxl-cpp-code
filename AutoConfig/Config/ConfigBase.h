#pragma once
#include <string>
#include <vector>
#include <map>
// #include "Config/ConfigUtil.h"
#include "ConfigEx/ConfigExtra.h"

namespace Config
{
    class ConfigBase
    {
    public:
        template <typename T>
        const T GetField(std::string field_name)
        {
            static_assert(false, "need specialization this funtion");
            return T();
        }

        template <>
        const int GetField<int>(std::string field_name)
        {
            return 2;
        }

        template <>
        const float GetField<float>(std::string field_name)
        {
            return 2;
        }

    private:
        int m_int_val;
        float m_float_val;
        std::vector<int> m_int_vec;
        std::map<int, int> m_int_int_map;
        ConfigEx::ConfigExtra m_extra_data;

    public:
        int int_valx() { return m_int_val; }
        float float_valx() { return m_float_val; }
        const std::vector<int> & int_vecx() { return m_int_vec; }
        const std::map<int, int> & int_int_mapx() { return m_int_int_map; }
        const ConfigEx::ConfigExtra & extra_data() { return m_extra_data; }

    public:
        const char * Field_Name_int_val = "int_val";
        const char * Field_Name_float_val = "float_val";
        const char * Field_Name_int_vec = "int_vec";
        const char * Field_Name_int_int_map = "int_int_map";

        using CheckLineFunc = bool (*)(ConfigBase *item);
        bool Init(std::map<std::string, std::string> kvPairs, CheckLineFunc func)
        {
            bool all_ok = true;
            all_ok = all_ok && kvPairs.count(Field_Name_int_val) > 0 && (m_int_val = std::stoi(kvPairs[Field_Name_int_val]), true);
            all_ok = all_ok && kvPairs.count(Field_Name_float_val) > 0 && (m_float_val = std::stof(kvPairs[Field_Name_float_val]), true);
            all_ok = all_ok && kvPairs.count(Field_Name_int_vec) > 0 && (m_int_val = std::stoi(kvPairs[Field_Name_int_vec]), true);
            all_ok = all_ok && kvPairs.count(Field_Name_int_val) > 0 && (m_int_val = std::stoi(kvPairs[Field_Name_int_val]), true);

            if (all_ok && nullptr != func)
            {
                all_ok &= func(this);
            }
        }
    };
}
