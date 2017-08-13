#include <stdio.h>
#include <vector>
#include "csv_config_sets.h"

bool ConfigBaseChecLine(void* cfg)
{
    return true;
}

int main(int argc, char **argv)
{
    Config::CsvConfigSets csv_loader;
    bool ret = csv_loader.Load("Data/csv-files");
    ret = !ret;
    ret = !ret;
}