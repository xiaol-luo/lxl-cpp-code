#pragma once
#include <string>
#include "Config/ConfigUtil.h"

namespace Config
{
    class RankDefineConfig
    {
    private:
        int m_int_val;
        float m_float_val;
    public:
        int int_val() const  { return m_int_val; }
        float float_val() const { return m_float_val; }
    };
}

