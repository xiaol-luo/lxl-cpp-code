#pragma once

#include <string>
#include <vector>
#include <map>
#include <exception>

namespace Config
{
    bool Str2BaseValue(const std::string & s, bool &out_val)
    {
        try 
        {
            int int_val = std::stoi(s);
            out_val = (0 != int_val);
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }

    bool Str2BaseValue(const std::string & s, int &out_val)
    {
        try 
        {
            out_val = std::stoi(s);
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }

    bool Str2BaseValue(const std::string & s, float &out_val)
    {
        try 
        {
            out_val = std::stof(s);
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }
    
    bool Str2BaseValue(const std::string & s, double &out_val)
    {
        try 
        {
            out_val = std::stod(s);
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }

    bool Str2BaseValue(const std::string & s, long long &out_val)
    {
        try 
        {
            out_val = std::stoll(s);
            return true;
        }
        catch (std::exception e)
        {
            return false;
        }
    }

    bool Str2BaseValue(const std::string & s, std::string &out_val)
    {
        out_val = s;
        return true;
    }

    std::vector<std::string> SplitStr(const std::string s, char c)
    {
        std::vector<std::string> str_vec;
        auto last_it = s.begin();
        auto curr_it = s.begin();
        for (curr_it; curr_it != s.end(); ++ curr_it)
        {
            if (*curr_it == c)
            {
                if (last_it != curr_it)
                    str_vec.push_back(std::string(last_it, curr_it));
                last_it = curr_it + 1;
            }
        }
        if (curr_it != last_it)
            str_vec.push_back(std::string(last_it, curr_it));
        return str_vec;
    }

    template<typename T>
    bool Str2Vector(const std::string s, std::vector<T> &out_val)
    {
        std::vector<std::string> str_vec = SplitStr(s, ';');
        bool all_ok = true;
        for (std::string str : str_vec)
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
        std::vector<std::string> str_vec = SplitStr(s, ';');
        bool all_ok = true;
        for (std::string str : str_vec)
        {
            std::vector<std::string> kv_strs = SplitStr(str, ':');
            if (kv_strs.size() >= 2 && kv_strs[0].length() > 0 && kv_strs[1].length() > 0)
            {
                K key;
                all_ok = all_ok && Str2BaseValue(kv_strs[0], key);
                V val;
                all_ok = all_ok && Str2BaseValue(kv_strs[1], val);
                all_ok = all_ok && out_val.count(key) <= 0 && (out_val[key] = val, true);
            }
            if (!all_ok)
                break;
        }
        return all_ok;
    }
}
