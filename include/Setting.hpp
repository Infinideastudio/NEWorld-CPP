//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_SETTING_HPP
#define NEWORLD_SETTING_HPP

#include "JsonKeys.hpp"

#include <string>

#include "rapidjson/document.h"

namespace JSON {
extern rapidjson::Document Resources;
extern rapidjson::Document Settings;
extern rapidjson::Document Infomation;
}  // namespace json

template <typename T>
void GetValue(
    const rapidjson::Document &document,
    const std::string &key,
    T &dest
);

void ReadSettings();

bool LoadResourcesJSON(const std::string &jsonPath = ResourcesFile);
void SaveAllJSON();

bool LoadJSONFromKey(
    const rapidjson::Document &keySrc,
    rapidjson::Document &dest,
    const std::string &key
);
void SaveJSONFromKey(
    const rapidjson::Document &keySrc,
    const rapidjson::Document &document,
    const std::string &key
);
void SaveJSON(
    const rapidjson::Document &document,
    const std::string &destFile
);

#endif  // NEWORLD_SETTING_HPP
