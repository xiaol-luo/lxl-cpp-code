#include <stdio.h>
#include <vector>
#include "CsvParser/csv.h"
#include "auto-config/config_base.h"
#include "csv_config_sets.h"

bool ConfigBaseChecLine(void* cfg)
{
    return true;
}

int main(int argc, char **argv)
{
    Config::ConfigBaseSet set;
    set.Load("config_base.csv");

    Config::CsvConfigSets csv_loader;
    bool ret = csv_loader.Load("csv-files");
    ret = !ret;
    ret = !ret;
}