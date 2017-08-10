#pragma once
namespace ConfigEx
{
    class ConfigExtra
    {
    public:
        int int_val;
        float float_val;

    public:
        template <typename T>
        bool Init(const T & cfg_elem)
        {
            int_val = cfg_elem.int_val;
            float_val = cfg_elem.float_val;
            return true;
        }
    };
}

