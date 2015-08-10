//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_LOG_SYSTEM_HPP
#define NEWORLD_LOG_SYSTEM_HPP

#include "Tools.hpp"

#include <mutex>
#include <bitset>
#include <string>
#include <fstream>
#include <thread>
#include <iostream>
#include <unordered_map>

#define FMT_HEADER_ONLY
#include "cppformat/format.h"

constexpr int NUMBER_OF_LOGTYPE = 7;

enum class LogType {
    Info = 0,
    Warning = 1,
    Error = 2,
    Fatal = 3,
    Debug = 4,
    Trace = 5,
    Unknown = 6
};  // enum class LogType

// 可以直接根据LogType索引到日志类型名称
constexpr char const *LogTypeString[] = {
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "DEBUG",
    "TRACE",
    "UNKNOWN"
};

class LogSystem {
public:
    LogSystem() = delete;
    LogSystem(const LogSystem &) = delete;
    LogSystem(LogSystem &&) = delete;
    ~LogSystem() = delete;
    LogSystem &operator=(const LogSystem&) = delete;
    LogSystem &operator=(LogSystem&&) = delete;

    static void SetLogDirectory(const std::string &directory);

    template <typename ... Args>
    static void Info(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    static void Warning(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    static void Error(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    static void Fatal(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    static void Debug(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    static void Trace(const std::string &fmt, const Args ... args);

    static void SetThreadName(const std::string &name);
    static void SetRegionName(const std::string &name);

    static void SetOutput(bool logToFile = true, bool logToStdout = true);

    static void DisableLog(const LogType &logType);
    static void EnableLog(const LogType &logType);
    static void DisableAll();
    static void EnableAll();

    static bool IsVaild();

    static void CopyToLatest();

private:
    template <typename ... Args>
    static void _Log(const std::string &fmt, const LogType &logType, const Args ... args);

    static void AddSelectionInMessage(std::string &buf, const std::string &value);
    static std::string FindThreadName();

    static bool m_logToFile;
    static bool m_logToStdout;
    static std::string m_llogFileName;
    static std::string m_regionName;
    static std::ofstream m_logFile;
    static std::bitset<NUMBER_OF_LOGTYPE> m_logFiliter;
    static std::mutex m_writeMtx;
    static std::mutex m_threadNameMtx;  // 防止出现插入冲突的情况
    static std::unordered_map<std::thread::id, std::string> m_threadMap;
};  // class LogSystem


template <typename ... Args>
void LogSystem::Info(const std::string &fmt, const Args ... args) {
    _Log(fmt, LogType::Info, args...);
}


template <typename ... Args>
void LogSystem::Warning(const std::string &fmt, const Args ... args) {
    _Log(fmt, LogType::Warning, args...);
}


template <typename ... Args>
void LogSystem::Error(const std::string &fmt, const Args ... args) {
    _Log(fmt, LogType::Error, args...);
}


template <typename ... Args>
void LogSystem::Fatal(const std::string &fmt, const Args ... args) {
    _Log(fmt, LogType::Fatal, args...);
}


template <typename ... Args>
void LogSystem::Debug(const std::string &fmt, const Args ... args) {
    _Log(fmt, LogType::Debug, args...);
}


template <typename ... Args>
void LogSystem::Trace(const std::string &fmt, const Args ... args) {
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

    std::string buf;
    buf.reserve(MessageNormalSize);

    // 格式：[TIME][REGION NAME][THREAD NAME][MESSAGE TYPE] MESSAGE\n\t ...
    // 每个空行后默认带一个缩进

    AddSelectionInMessage(buf, GetTimeString());
    AddSelectionInMessage(buf, m_regionName);
    AddSelectionInMessage(buf, FindThreadName());
    AddSelectionInMessage(buf, LogTypeString[static_cast<unsigned>(logType)]);

    buf += ' ';
    buf += fmt::format(fmt, args...);  // 使用cppformat

    for (std::string::size_type i = 0;
            i != buf.size() - 1;
            ++i) {
        if (buf[i] == '\n') {
            buf.insert(i + 1, "\t");  // 插入缩进
        }
    }  // for

    // 保证写入的数据不被打乱
    std::lock_guard<std::mutex> lock(m_writeMtx);

    if (m_logToStdout) {
        std::cout.clear();
        std::cout << buf << std::endl;
    }

    if (m_logToFile) {
        if (!m_logFile.is_open()) {
            throw std::runtime_error("Log file invaild! Have you ever set log file directory?");
        } else {
            m_logFile.clear();
            m_logFile << buf << std::endl;
            m_logFile.flush();  // 自动刷新
        }
    }
}


#endif  // NEWORLD_LOG_SYSTEM_HPP
