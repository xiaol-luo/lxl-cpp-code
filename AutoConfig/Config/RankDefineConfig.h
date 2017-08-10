#pragma once
#include <string>

class RankDefineConfig
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
};
