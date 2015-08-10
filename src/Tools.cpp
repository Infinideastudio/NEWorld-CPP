//
// Copyright 2015 infinideas
//

#include "../include/Tools.hpp"

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
    if (!filesystem::exists(filePath)) {
        throw invalid_argument("filePath");
    }

    ifstream fileReader(filePath);
    if (!fileReader.is_open()) {
        throw runtime_error("Cannot open file: " + filePath);
    }

    string buf;
    while (fileReader.eof()) {
        std::getline(fileReader, buf);
        buf += '\n';
    }

    return buf;
}
