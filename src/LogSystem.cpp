//
// Copyright 2015 infinideastudio
//

#include "../include/LogSystem.hpp"

#include "../include/Tools.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "boost/filesystem.hpp"

#define FMT_HEADER_ONLY
#include "../include/cppformat/format.h"

using namespace std;
using namespace boost;

void LogSystem::SetLogDirectory(const string &directory) {
    // 确保目录存在
    if (!filesystem::exists(directory)) {
        filesystem::create_directories(directory);
    }

    unsigned cnt = 1;
    string today = GetDateString();

    while (filesystem::exists(today + '-' + to_string(cnt) + ".log")) {
        ++cnt;
    }   // while

    m_logFile.open(today + '-' + to_string(cnt) + ".log");

    // 可能权限不够导致文件打开失败
    if (!m_logFile.is_open()) {
        throw runtime_error("Failed to open log file. Unknown reason...");
    }
}


template <typename ... Args>
void LogSystem::Info(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Info, args...);
}


template <typename ... Args>
void LogSystem::Warning(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Warning, args...);
}


template <typename ... Args>
void LogSystem::Error(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Error, args...);
}


template <typename ... Args>
void LogSystem::Fatal(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Fatal, args...);
}


template <typename ... Args>
void LogSystem::Debug(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Debug, args...);
}


template <typename ... Args>
void LogSystem::Trace(const string &fmt, const Args ... args) {
    _Log(fmt, LogType::Trace, args...);
}


// 日志记录的真正实现
template <typename ... Args>
void LogSystem::_Log(const std::string &fmt, const LogType &logType, const Args ... args) {
    // 过滤信息
    if (m_logFiliter.test(static_cast<unsigned>(logType))) {
        return;
    }

    // 日志信息的通常大小
    constexpr int MessageNormalSize = 80;

    string buf;
    buf.reserve(MessageNormalSize);

    // 格式：[TIME][REGION NAME][THREAD NAME][MESSAGE TYPE] MESSAGE\n\t ...
    // 每个空行后默认带一个缩进

    AddSelectionInMessage(buf, GetTimeString());
    AddSelectionInMessage(buf, m_regionName);
    AddSelectionInMessage(buf, FindThreadName());
    AddSelectionInMessage(buf, LogTypeString[static_cast<unsigned>(logType)]);

    buf += ' ';
    buf += format(fmt, args...);  // 使用cppformat

    for (string::size_type i = 0;
            i != buf.size() - 1;
            ++i) {
        if (buf[i] == '\n') {
            buf.insert(i + 1, "\t");  // 插入缩进
        }
    }  // for

    // 保证写入的数据不被打乱
    lock_guard<mutex> lock(m_writeMtx);

    if (m_logToStdout) {
        cout.clear();
        cout << buf << endl;
    }

    if (m_logToFile) {
        if (!m_logFile.is_open()) {
            throw runtime_error("Log file invaild! Have you ever set log file directory?");
        } else {
            m_logFile.clear();
            m_logFile << buf << endl;
            m_logFile.flush();  // 自动刷新
        }
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
