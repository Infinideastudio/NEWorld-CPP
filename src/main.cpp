//
// Copyright 2015 infinideas
//

#include "../include/global.hpp"

#include "../include/logging/LogSystem.hpp"

#include "../include/events/EventSystem.hpp"

#include "../include/threading/Thread.hpp"

#include "../include/NativeSupport.hpp"
#include "../include/EventsImpl.hpp"
#include "../include/TickCounter.hpp"
#include "../include/ExceptionHandler.hpp"
#include "../include/Setting.hpp"
#include "../include/Render.hpp"
#include "../include/Update.hpp"
#include "../include/Tools.hpp"
#include "../include/LogFucker.hpp"

#include <cstdlib>
#include <atomic>

#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace std;

int main(/*int argc, char *argv[]*/) {
    // 载入日志
    LogSystem::SetLogDirectory(GetCurrentDirectory() + "logs/");
    LogSystem::SetRegionName("Client");
    LogSystem::SetThreadName("Main Thread");
    LogSystem::SetOutput(true, true);
    atexit(LogSystem::CopyToLatest);

    LogSystem::Debug("Hello, programmers! What a ****ing day for debugging!");
    LogSystem::Debug("See today's ****ing.");
    LogFucker::Fuck();
    LogSystem::Info("Log system started.");
    LogSystem::Debug("Platform: {}", GetPlatform());

    // 绑定事件
    LogSystem::Info("Connecting events...");
    ConnectEvents();

    // 载入配置
    LogSystem::Info("Loading settings...");
    if (!LoadResourcesJSON()) { LogSystem::Error("Failed to load {}.", ResourcesFile); } else {
        LogSystem::Info("Reading settings...");
        ReadSettings();
        atexit(SaveAllJSON);
        LogSystem::Info("Settings read!");
    }

    // 设置定制的terminate_handler
    SetExcetopnHandler();

    // 启动线程
    LogSystem::Info("Starting threads...");
    Thread renderThread(RenderPrepare, Rendering, RenderCleanup);
    Thread updateThread(UpdatePrepare, Updating, UpdateCleanup);

    renderThread.Start();
    updateThread.Start();

    LogSystem::Debug("Waiting for threads...");
    renderThread.WaitForPrepare();
    updateThread.WaitForPrepare();

    LogSystem::Debug("Checking threads' status...");
    LogSystem::Debug("Render thread: {}", renderThread.GetStatus() == ThreadStatus::ExitedWithException ? "ERROR" : "NORMAL");
    LogSystem::Debug("Update thread: {}", renderThread.GetStatus() == ThreadStatus::ExitedWithException ? "ERROR" : "NORMAL");

    if (renderThread.GetStatus() == ThreadStatus::ExitedWithException || updateThread.GetStatus() == ThreadStatus::ExitedWithException) {
        LogSystem::Error("Cannot start threads! Program will exit.");
        flag = false;
    }

    // 主循环（事件循环）
    LogSystem::Info("Start main loop.");

    while (flag) {
        DoEvents();
        this_thread::sleep_for(chrono::milliseconds(EVENT_THREAD_SLEEP_FOR));  // 不要太激进了！！！
    }                                                                          // while

    LogSystem::Info("Exited main loop.");

    // 结束
    // 先结束线程
    LogSystem::Info("Exiting threads...");
    updateThread.Stop();
    renderThread.Stop();

    LogSystem::Info("Program exited.");

    LogSystem::Debug("Have problems been solved?");

    return 0;
}  // function main

