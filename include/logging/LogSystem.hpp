//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_LOG_SYSTEM_HPP
#define NEWORLD_LOG_SYSTEM_HPP

#include "../Tools.hpp"

#include <mutex>
#include <bitset>
#include <string>
#include <fstream>
#include <thread>
#include <iostream>
#include <unordered_map>

#define FMT_HEADER_ONLY
#include "../cppformat/format.h"

#include <boost/filesystem.hpp>

constexpr int NUMBER_OF_LOGTYPE = 7;

enum class LogType { Info = 0, Warning = 1, Error = 2, Fatal = 3, Debug = 4, Trace = 5, Unknown = 6 };  // enum class LogType

// 可以直接根据LogType索引到日志类型名称
constexpr char const *LogTypeString[] = { "INFO", "WARN", "ERROR", "FATAL", "DEBUG", "TRACE", "UNKNOWN" };

class LogSystem final {
 public:
    LogSystem() = delete;
    LogSystem(const LogSystem &) = delete;
    LogSystem(LogSystem &&) = delete;
    ~LogSystem() = delete;
    LogSystem &operator=(const LogSystem &) = delete;
    LogSystem &operator=(LogSystem &&) = delete;

    /**
     * 设置日志文件的存放目录
     * @param directory 目录
     * @remark:
     * 会立即生成日志文件
     */
    static void SetLogDirectory(const std::string &directory);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Info(const std::string &fmt, const Args... args);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Warning(const std::string &fmt, const Args... args);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Error(const std::string &fmt, const Args... args);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Fatal(const std::string &fmt, const Args... args);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Debug(const std::string &fmt, const Args... args);

    /**
     * 写入日志
     * @param fmt  格式化的日志内容
     * @param args 格式化参数
     */
    template <typename... Args>
    static void Trace(const std::string &fmt, const Args... args);

    /**
     * 给当前的线程(调用者)设置一个名字
     * @param name [description]
     */
    static void SetThreadName(const std::string &name);

    /**
     * 设置当前领域名称,如Client, Server...
     * @param name 名称
     */
    static void SetRegionName(const std::string &name);

    /**
     * 设置日志输出
     * @param logToFile   指示是否向文件输出
     * @param logToStdout 指示是否向控制台输出
     */
    static void SetOutput(bool logToFile = true, bool logToStdout = true);

    /**
     * 禁止某一类别的日志输出
     * @param logType 日志类别
     */
    static void DisableLog(const LogType &logType);

    /**
     * 开启某一类别的日志输出
     * @param logType 日志类别
     */
    static void EnableLog(const LogType &logType);

    /**
     * 关闭所有日志输出
     */
    static void DisableAll();

    /**
     * 打开所有日志输出
     */
    static void EnableAll();

    /**
     * 确认日志系统是否有效
     * @return 返回一个布尔值
     */
    static bool IsVaild();

    /**
     * 将当前日志复制到latest.log
     */
    static void CopyToLatest();

 private:
    template <typename... Args>
    static void _Log(const std::string &fmt, const LogType &logType, const Args... args);

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

// 模板大法好!!!!

template <typename... Args>
void LogSystem::Info(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_INFO
    _Log(fmt, LogType::Info, args...);
#endif
}

template <typename... Args>
void LogSystem::Warning(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_WARNING
    _Log(fmt, LogType::Warning, args...);
#endif
}

template <typename... Args>
void LogSystem::Error(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_ERROR
    _Log(fmt, LogType::Error, args...);
#endif
}

template <typename... Args>
void LogSystem::Fatal(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_FATAL
    _Log(fmt, LogType::Fatal, args...);
#endif
}

template <typename... Args>
void LogSystem::Debug(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_DEBUG
    _Log(fmt, LogType::Debug, args...);
#endif
}

template <typename... Args>
void LogSystem::Trace(const std::string &fmt, const Args... args) {
#ifndef LOG_NO_TRACE
    _Log(fmt, LogType::Trace, args...);
#endif
}

// 日志记录的真正实现
template <typename... Args>
void LogSystem::_Log(const std::string &fmt, const LogType &logType, const Args... args) {
    // 过滤信息
    if (m_logFiliter.test(static_cast<unsigned>(logType))) { return; }

    // 日志信息的通常大小
    constexpr int MessageNormalSize = 80;

    std::string msgBuffer;
    msgBuffer.reserve(MessageNormalSize);

    // 格式：[TIME][REGION NAME][THREAD NAME][MESSAGE TYPE] MESSAGE\n\t ...
    // 每个空行后默认带一个缩进

    AddSelectionInMessage(msgBuffer, GetTimeString());
    AddSelectionInMessage(msgBuffer, m_regionName);
    AddSelectionInMessage(msgBuffer, FindThreadName());
    AddSelectionInMessage(msgBuffer, LogTypeString[static_cast<unsigned>(logType)]);

    msgBuffer += ' ';
    msgBuffer += fmt::format(fmt, args...);  // 使用cppformat

    // 每个换行后添加一个缩进
    for (std::string::size_type i = 0; i != msgBuffer.size() - 1; ++i) {
        if (msgBuffer[i] == '\n') {
            msgBuffer.insert(i + 1, "\t");  // 插入缩进
        }
    }  // for

    // 保证写入的数据不被打乱
    std::lock_guard<std::mutex> lock(m_writeMtx);

    if (m_logToStdout) {
        std::cout.clear();
        std::cout << msgBuffer << std::endl;
    }

    if (m_logToFile) {
        if (!m_logFile.is_open()) { throw std::runtime_error("Log file invaild! Have you ever set log file directory?"); } else {
            m_logFile.clear();
            m_logFile << msgBuffer << std::endl;
            m_logFile.flush();  // 自动刷新
        }
    }
}

#endif  // NEWORLD_LOG_SYSTEM_HPP
