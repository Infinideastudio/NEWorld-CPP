//
// Copyright 2015 infinideas
//

#include "../include/LogSystem.hpp"

#include <cwchar>

#include "boost/filesystem.hpp"

using namespace std;
using namespace boost;

bool LogSystem::m_logToFile;
bool LogSystem::m_logToStdout;
string LogSystem::m_llogFileName;
string LogSystem::m_regionName;
ofstream LogSystem::m_logFile;
bitset<NUMBER_OF_LOGTYPE> LogSystem::m_logFiliter;
mutex LogSystem::m_writeMtx;
mutex LogSystem::m_threadNameMtx;
unordered_map<thread::id, string> LogSystem::m_threadMap;

void LogSystem::SetLogDirectory(const string &directory) {
    // 确保目录存在
    if (!filesystem::exists(directory)) {
        filesystem::create_directories(directory);
    }

    // 如果有latest.log，则先删除、
    if (filesystem::exists(directory + "latest.log")) {
        filesystem::remove(directory + "latest.log");
    }

    unsigned cnt = 1;
    string today = GetDateString();

    while (filesystem::exists(directory + today + '-' + to_string(cnt) + ".log")) {
        ++cnt;
    }   // while

    m_llogFileName = directory + today + '-' + to_string(cnt) + ".log";
    m_logFile.open(m_llogFileName);

    // 可能权限不够导致文件打开失败
    if (!m_logFile.is_open()) {
        throw runtime_error("Failed to open log file. Unknown reason...");
    }
}


void LogSystem::AddSelectionInMessage(string &buf, const string &value) {
    // 如果value为空则不加入
    if (value.empty()) {
        return;
    }

    buf += '[';
    buf += value;
    buf += ']';
}


string LogSystem::FindThreadName() {
    auto iter = m_threadMap.find(this_thread::get_id());

    if (iter == m_threadMap.end()) {
        return string();  // 返回空字符串
    }

    return iter->second;
}


void LogSystem::SetThreadName(const string &name) {
    auto iter = m_threadMap.find(this_thread::get_id());

    if (iter != m_threadMap.end()) {
        iter->second = std::move(name);
    } else {
        lock_guard<mutex> guard(m_threadNameMtx);

        m_threadMap.insert({this_thread::get_id(), std::move(name)});
    }
}


void LogSystem::SetRegionName(const std::string &name) {
    m_regionName = name;
}


void LogSystem::SetOutput(bool logToFile, bool logToStdout) {
    m_logToFile = logToFile;
    m_logToStdout = logToStdout;
}


void LogSystem::DisableLog(const LogType &logType) {
    m_logFiliter.set(static_cast<unsigned>(logType));
}


void LogSystem::EnableLog(const LogType &logType) {
    m_logFiliter.reset(static_cast<unsigned>(logType));
}


void LogSystem::DisableAll() {
    m_logFiliter.set();
}


void LogSystem::EnableAll() {
    m_logFiliter.reset();
}


bool LogSystem::IsVaild() {
    return m_logFile.is_open();
}

void LogSystem::CopyToLatest() {
    if (m_llogFileName.empty()) {
        throw runtime_error("No log file.");
    }

    if (!filesystem::exists(m_llogFileName)) {
        throw runtime_error("Log file does not exist.");
    }

    filesystem::path latest = filesystem::path(m_llogFileName).parent_path();
    latest /= "latest.log";

    if (filesystem::exists(latest)) {
        filesystem::remove(latest);
    }

    filesystem::copy(m_llogFileName, latest);
}
