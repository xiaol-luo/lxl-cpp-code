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
        bool Init(T cfg_elem)
        {
            int_val = cfg_elem.GetField<int>("int_val");
            float_val = cfg_elem.GetField<float>("float_val");
            return true;
        }
    };
}

