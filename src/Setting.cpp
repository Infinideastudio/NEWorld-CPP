//
// Copyright 2015 infinideas
//

#include "../include/Setting.hpp"

#include "../include/LogSystem.hpp"
#include "../include/Tools.hpp"

#include <fstream>

#include <boost/filesystem.hpp>

#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"

using namespace std;
using namespace boost;
using namespace rapidjson;

namespace JSON {
Document Resources;
Document Settings;
Document Infomation;
}  // namespace json

void ReadSettings() {
    // TODO(riteme): Complete this.
}


bool LoadResourcesJSON(const string &jsonPath) {
    // bool status = true;

    // 确认资源文件存在
    if (!filesystem::exists(jsonPath)) {
        LogSystem::Error("Resources file not found.");
        return false;
    }

    // 尝试读取
    try {
        string jsonBuf = ReadFile(jsonPath);
    } catch (...) {
        LogSystem::Error("Cannot read resources file.");
        return false;
    }

    // 解析JSON
    JSON::Resources.Parse(jsonPath.c_str());

    // 如果发生错误则推出
    if (JSON::Resources.HasParseError()) {
        LogSystem::Error("Cannot parse resources file.");
        return false;
    }

    bool status = true;

    // 尝试读取信息文件
    LoadJSONFromKey(JSON::Resources, JSON::Infomation, InformationKey);

    // 尝试读取设置
    LoadJSONFromKey(JSON::Resources, JSON::Settings, SettingsKey);

    return status;
}


bool SaveAllJSON() {
    SaveJSON(JSON::Resources, ResourcesFile);

    SaveJSONFromKey(JSON::Infomation, InformationKey);
    SaveJSONFromKey(JSON::Settings, SettingsKey);

    return true;
}


bool LoadJSONFromKey(
    const Document &keySrc,
    Document &dest,
    const string &key
) {
    if (
        keySrc.HasMember(key.c_str()) &&
        keySrc[key.c_str()].IsString()
    ) {
        string json = keySrc[key.c_str()].GetString();

        if (filesystem::exists(json)) {
            string buf = ReadFile(json);
            dest.Parse(buf.c_str());

            if (dest.HasParseError()) {
                LogSystem::Error("Cannot parse file: {}.", json);
                return false;
            }

        } else {
            LogSystem::Error("File not found: {}", json);
            return false;
        }
    } else {
        LogSystem::Error("No file specfied: {}", key);
        return false;
    }

    return true;
}


void SaveJSONFromKey(
    const rapidjson::Document &document,
    const std::string &key
) {
    // 如果没有则取消写入
    if (!document.HasMember(key.c_str())) {
        LogSystem::Warning("Key not found: {}", key);
        return;
    }

    if (!document[key.c_str()].IsString()) {
        LogSystem::Warning("Key type error: {}", key);
        return;
    }

    string json = document[key.c_str()].GetString();

    SaveJSON(document, json);
}


void SaveJSON(
    const Document &document,
    const string &destFile
) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    ofstream fileWriter(destFile, ios::trunc);
    fileWriter << buffer.GetString();
    fileWriter.close();
}
