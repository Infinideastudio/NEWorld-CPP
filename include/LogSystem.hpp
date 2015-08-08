//
// Copyright 2015 infinideastudio
//

#ifndef NEWORLD_LOG_SYSTEM_HPP
#define NEWORLD_LOG_SYSTEM_HPP

#include <mutex>
#include <bitset>
#include <string>
#include <fstream>
#include <thread>
#include <unordered_map>

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

    void SetLogDirectory(const std::string &directory);

    template <typename ... Args>
    void Info(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    void Warning(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    void Error(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    void Fatal(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    void Debug(const std::string &fmt, const Args ... args);

    template <typename ... Args>
    void Trace(const std::string &fmt, const Args ... args);

    void SetThreadName(const std::string &name);
    void SetRegionName(const std::string &name);

    void SetOutput(bool logToFile = true, bool logToStdout = true);

    void DisableLog(const LogType &logType);
    void EnableLog(const LogType &logType);
    void DisableAll();
    void EnableAll();

private:
    template <typename ... Args>
    void _Log(const std::string &fmt, const LogType &logType, const Args ... args);
    void AddSelectionInMessage(std::string &buf, const std::string &value);
    std::string FindThreadName();

    bool m_logToFile = true;
    bool m_logToStdout = true;
    std::string m_regionName;
    std::ofstream m_logFile;
    std::bitset<NUMBER_OF_LOGTYPE> m_logFiliter;
    std::mutex m_writeMtx;
    std::unordered_map<std::thread::id, std::string> m_threadMap;
};  // class LogSystem

#endif  // NEWORLD_LOG_SYSTEM_HPP
