#pragma once

#include <string>
#include <vector>
#include <map>
#include <exception>

using ConfigCheckFunc = bool (*)(void* item);
using ConfigSetCheckFunc = bool (*)(void* item);

namespace Config
{
    bool Str2BaseValue(const std::string & s, bool &out_val);
    bool Str2BaseValue(const std::string & s, int &out_val);
    bool Str2BaseValue(const std::string & s, float &out_val);
    bool Str2BaseValue(const std::string & s, double &out_val);
    bool Str2BaseValue(const std::string & s, long long &out_val);
    bool Str2Str(const std::string & s, std::string &out_val);
    std::vector<std::string> SplitStr(const std::string s, char c);

    template<typename T>
    bool Str2Vec(const std::string s, std::vector<T> &out_val)
    {
        // v1; v2
        bool all_ok = true;
        for (std::string str : SplitStr(s, ';'))
        {
            T tmp_val;
            if (!Str2BaseValue(str, tmp_val))
            {
                all_ok = false;
                break;
            }
            out_val.push_back(tmp_val);
        }
        return all_ok;
    }

    template<typename K, typename V>
    bool Str2Map(const std::string s, std::map<K, V> &out_val)
    {
        // k1:v1; k2:v2
        bool all_ok = true;
        for (std::string str : SplitStr(s, ';'))
        {
            std::vector<std::string> kv_strs = SplitStr(str, ':');
            if (kv_strs.size() < 2 || kv_strs[0].length() <= 0 || kv_strs[1].length() <= 0)
                all_ok = false;
            K key;
            all_ok = all_ok && Str2BaseValue(kv_strs[0], key);
            V val;
            all_ok = all_ok && Str2BaseValue(kv_strs[1], val);
            all_ok = all_ok && out_val.count(key) <= 0 && (out_val[key] = val, true);
            if (!all_ok)
                break;
        }
        return all_ok;
    }

    template<typename T>
    bool Str2VecVec(const std::string s, std::vector<std::vector<T>> &out_val)
    {
        // a1|a2; b1|b2
        bool all_ok = true;
        for (std::string vec_str : SplitStr(s, ';'))
        {
            std::vector<T> val_vec;
            for (std::string val_str : SplitStr(vec_str, '|'))
            {
                T tmp_val;
                all_ok = all_ok && Str2BaseValue(val_str, tmp_val);
                if (!all_ok)
                    break;
                val_vec.push_back(tmp_val);
            }
            if (!all_ok)
                break;
            out_val.push_back(val_vec);
        }
        return all_ok;
    }

    template <typename K, typename V>
    bool Str2MapVec(const std::string s, std::map<K, std::vector<V>> &out_val)
    {
        // k1:v1|v2; kk1: vv1|vv2
        bool all_ok = true;
        for (std::string vec_str : SplitStr(s, ';'))
        {
            std::vector<std::string> kv_strs = SplitStr(vec_str, ':');
            if (kv_strs.size() < 2 || kv_strs[0].length() <= 0)
                all_ok = false;
            K key;
            all_ok = all_ok && Str2BaseValue(kv_strs[0], key);
            std::vector<V> val_vec;
            for (std::string val_str : SplitStr(kv_strs[1], '|'))
            {
                V tmp_val;
                all_ok = all_ok && Str2BaseValue(val_str, tmp_val);
                if (!all_ok)
                    break;
                val_vec.push_back(tmp_val);
            }
            all_ok = all_ok && out_val.count(key) <= 0 && (out_val[key] = val_vec, true);
            if (!all_ok)
                break;
        }
        return all_ok;
    };
}
