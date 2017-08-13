#include <stdio.h>
#include <vector>
#include "csv_config_sets.h"
#include "IniFile/IniFile.hpp"
#include <sstream>

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

    std::istringstream ss(("[Foo]\nbar=hello world\n[Test]"));
    ini::IniFile inif(ss);
    std::string ini_str = inif["Foo"]["bar"].asString();
}