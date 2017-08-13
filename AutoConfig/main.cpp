#include <stdio.h>
#include <vector>
#include "csv_config_sets.h"
#include "IniFile/IniFile.hpp"
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidxml/rapidxml.hpp"

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

    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(json);

    rapidxml::xml_document<> xml_doc;
}