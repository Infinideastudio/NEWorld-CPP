//
// Copyright 2015 infinideas
//

#include "../include/Setting.hpp"

#include "../include/LogSystem.hpp"
#include "../include/Tools.hpp"
#include "../include/global.hpp"

#include <fstream>

#include <boost/filesystem.hpp>

#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h"

using namespace std;
using namespace boost;
using namespace rapidjson;

namespace JSON {
Document Resources;
Document Settings;
Document Infomation;
}  // namespace json

template <>
void GetValue(
    const Document &document,
    const string &key,
    string &dest
) {
    if (document.HasMember(key.c_str())) {

        if (document[key.c_str()].IsNull()) {
            LogSystem::Debug("Value not read. Key: {}", key);
            return;
        }

        if (document[key.c_str()].IsString()) {
            dest = document[key.c_str()].GetString();
        } else {
            LogSystem::Warning("The value to key {} isn't string.", key);
        }
    } else {
        LogSystem::Warning("Key not found: {}", key);
    }
}


template <>
void GetValue(
    const Document &document,
    const string &key,
    int &dest
) {
    if (document.HasMember(key.c_str())) {

        if (document[key.c_str()].IsNull()) {
            LogSystem::Debug("Value not read. Key: {}", key);
            return;
        }

        if (document[key.c_str()].IsInt()) {
            dest = document[key.c_str()].GetInt();
        } else {
            LogSystem::Warning("The value to key {} isn't int.", key);
        }
    } else {
        LogSystem::Warning("Key not found: {}", key);
    }
}


template <>
void GetValue(
    const Document &document,
    const string &key,
    bool &dest
) {
    if (document.HasMember(key.c_str())) {

        if (document[key.c_str()].IsNull()) {
            LogSystem::Debug("Value not read. Key: {}", key);
            return;
        }

        if (document[key.c_str()].IsBool()) {
            dest = document[key.c_str()].GetBool();
        } else {
            LogSystem::Warning("The value to key {} isn't bool.", key);
        }
    } else {
        LogSystem::Warning("Key not found: {}", key);
    }
}


template <>
void GetValue(
    const Document &document,
    const string &key,
    double &dest
) {
    if (document.HasMember(key.c_str())) {

        if (document[key.c_str()].IsNull()) {
            LogSystem::Debug("Value not read. Key: {}", key);
            return;
        }

        if (document[key.c_str()].IsDouble()) {
            dest = document[key.c_str()].GetDouble();
        } else {
            LogSystem::Warning("The value to key {} isn't double.", key);
        }
    } else {
        LogSystem::Warning("Key not found: {}", key);
    }
}


void ReadSettings() {
    GetValue(JSON::Infomation, VersionKey, VERSION);

    GetValue(JSON::Settings, WindowWidthKey, WindowWidth);
    GetValue(JSON::Settings, WindowHeightKey, WindowHeight);
}


bool LoadResourcesJSON(const string &jsonPath) {
    // bool status = true;

    // 确认资源文件存在
    if (!filesystem::exists(jsonPath)) {
        LogSystem::Error("Resources file not found.");
        return false;
    }

    // 尝试读取
    string jsonBuf;
    try {
        jsonBuf = ReadFile(jsonPath);
    } catch (...) {
        LogSystem::Error("Cannot read resources file.");
        return false;
    }

    LogSystem::Debug("./resources.json:\n{}", jsonBuf);

    // 解析JSON
    JSON::Resources.Parse(jsonBuf.c_str());

    // 如果发生错误则推出
    if (JSON::Resources.HasParseError()) {
        LogSystem::Error("Cannot parse resources file.");
        return false;
    }

    bool status = true;

    // 尝试读取信息文件
    LoadJSONFromKey(JSON::Resources, JSON::Infomation, InformationFileKey);

    // 尝试读取设置
    LoadJSONFromKey(JSON::Resources, JSON::Settings, SettingsFileKey);

    return status;
}


void SaveAllJSON() {
    SaveJSON(JSON::Resources, ResourcesFile);

    SaveJSONFromKey(JSON::Resources, JSON::Infomation, InformationFileKey);
    SaveJSONFromKey(JSON::Resources, JSON::Settings, SettingsFileKey);
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
    const Document &keySrc,
    const Document &document,
    const string &key
) {
    // 如果没有则取消写入
    if (!keySrc.HasMember(key.c_str())) {
        LogSystem::Warning("Key not found: {}", key);
        return;
    }

    if (!keySrc[key.c_str()].IsString()) {
        LogSystem::Warning("Key type error: {}", key);
        return;
    }

    string json = keySrc[key.c_str()].GetString();

    SaveJSON(document, json);
}


void SaveJSON(
    const Document &document,
    const string &destFile
) {
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    document.Accept(writer);

    ofstream fileWriter(destFile, ios::trunc);
    fileWriter << buffer.GetString();
    fileWriter.close();
}
