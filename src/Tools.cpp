//
// Copyright 2015 infinideas
//

#include "../include/Tools.hpp"

#include "../include/logging/LogSystem.hpp"

#include <ctime>
#include <utility>
#include <string>
#include <fstream>
#include <algorithm>

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;

std::string CorrectPath(const std::string &originPath, const PathType &destType) {
    char findChar, turnTo;

    if (destType == PathType::Windows) {
        findChar = '/';
        turnTo = '\\';
    } else {
        findChar = '\\';
        turnTo = '/';
    }

    string buf(std::move(originPath));
    replace(buf.begin(), buf.end(), findChar, turnTo);

    return buf;
}

std::string GetDateString() {
    constexpr unsigned TimeStringLength = 11;
    constexpr char TimeFormat[] = "%Y-%m-%d";

    char buf[TimeStringLength];
    time_t timer = time(nullptr);

    strftime(buf, sizeof(buf), TimeFormat, localtime(&timer));

    return string(buf);
}

std::string GetTimeString() {
    constexpr unsigned TimeStringLength = 9;
    constexpr char TimeFormat[] = "%H:%M:%S";

    char buf[TimeStringLength];
    time_t timer = time(nullptr);

    strftime(buf, sizeof(buf), TimeFormat, localtime(&timer));

    return string(buf);
}

string ReadFile(const string &filePath) {
    // 检查文件是否存在
    if (!filesystem::exists(filePath)) { throw invalid_argument("filePath"); }

    ifstream fileReader(filePath);
    if (!fileReader.is_open()) { throw runtime_error("Cannot open file: " + filePath); }

    string fileBuf;
    string lineBuf;
    while (getline(fileReader, lineBuf)) {
        fileBuf += lineBuf;
        fileBuf += '\n';
        fileReader.clear();
    }

    return fileBuf;
}

namespace {
#include <stdio.h>

#ifdef __WINDOWS__
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif
}  // namespace
std::string GetCurrentDirectory() {
    char buf[FILENAME_MAX];

    if (getcwd(buf, sizeof(buf))) {
        string directory(buf);

#ifdef __WINDOWS__
        directory += '\\';
#else
        directory += '/';
#endif  // IFDEF __WINDOWS__

        return directory;
    } else { return string("./"); }
}
