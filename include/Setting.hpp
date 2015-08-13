//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_SETTING_HPP
#define NEWORLD_SETTING_HPP

#include <string>

#include "rapidjson/document.h"

constexpr char ResourcesFile[] = "./resources.json";

constexpr char InformationKey[] = "information_file";
constexpr char SettingsKey[] = "setting_file";

namespace JSON {
extern rapidjson::Document Resources;
extern rapidjson::Document Settings;
extern rapidjson::Document Infomation;
}  // namespace json

void ReadSettings();

bool LoadResourcesJSON(const std::string &jsonPath = ResourcesFile);
bool SaveAllJSON();

bool LoadJSONFromKey(
    const rapidjson::Document &keySrc,
    rapidjson::Document &dest,
    const std::string &key
);
void SaveJSONFromKey(
    const rapidjson::Document &document,
    const std::string &key
);
void SaveJSON(
    const rapidjson::Document &document,
    const std::string &destFile
);

#endif  // NEWORLD_SETTING_HPP
