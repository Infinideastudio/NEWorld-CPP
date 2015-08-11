//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_TOOLS_HPP
#define NEWORLD_TOOLS_HPP

#include <string>

enum class PathType {
    Windows, POSIX
};  // enum class PathType

std::string CorrectPath(const std::string &originPath, const PathType &destType);

std::string GetDateString();
std::string GetTimeString();

std::string ReadFile(const std::string &filePath);

std::string GetCurrentDirectory();

#endif  // NEWORLD_TOOLS_HPP
