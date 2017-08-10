#include "Config/ConfigBase.h"

namespace Config
{
    const ConfigBase & ConfigBase::Default()
    {
        static ConfigBase cfg;
        return cfg;
    }
}



