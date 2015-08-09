//
// Copyright 2015 infinideas
//

#include "../include/Tools.hpp"

#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

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
