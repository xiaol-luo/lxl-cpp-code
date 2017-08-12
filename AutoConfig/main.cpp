#include <stdio.h>
#include <vector>
#include "CsvParser/csv.h"
#include "auto-config/config_base.h"

bool ConfigBaseChecLine(void* cfg)
{
    return true;
}

int main(int argc, char **argv)
{
    Config::ConfigBaseSet set;
    set.Load("config_base.csv");
}