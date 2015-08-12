//
// Copyright 2015 infinideas
//

#include "../include/global.hpp"

#include "../include/LogSystem.hpp"
#include "../include/NativeSupport.hpp"
#include "../include/EventSystem.hpp"
#include "../include/TickCounter.hpp"
#include "../include/Thread.hpp"
#include "../include/Render.hpp"
#include "../include/Tools.hpp"

#include <cstdlib>
#include <atomic>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

void Quit();
void GetFPS(float speed);

int main(/*int argc, char *argv[]*/) {
    // 载入日志
    LogSystem::SetLogDirectory(GetCurrentDirectory() + "logs/");
    LogSystem::SetRegionName("Client");
    LogSystem::SetThreadName("Main Thread");
    LogSystem::SetOutput(true, true);
    atexit(LogSystem::CopyToLatest);

    LogSystem::Debug("Hello, programmers! What a f**king day for debugging!");
    LogSystem::Info("Log system started.");

    // 绑定事件
    LogSystem::Info("Connecting signals with slots...");
    EventSystem::Connect(Events::ApplicationQuit, Quit);
    EventSystem::Connect(Events::FPSReport, GetFPS);

    // 启动线程
    LogSystem::Info("Creating threads...");
    Thread renderThread(
        RenderPrepare,
        Rendering,
        RenderCleanup
    );

    LogSystem::Info("Starting threads...");
    renderThread.Start();
    renderThread.WaitForPrepare();

    // 主循环（事件循环）
    LogSystem::Info("Start main loop.");

    EventSystem::StartEventSystem();
    while (flag) {
        EventSystem::DoEvents();
        this_thread::sleep_for(chrono::milliseconds(5));  // 不要太激进了！！！
    }   // while

    LogSystem::Info("Exited main loop.");

    // 结束
    // 先结束线程
    LogSystem::Info("Exiting threads");
    renderThread.Stop();

    EventSystem::StopEventSystem();

    LogSystem::Info("Program exited.");

    LogSystem::Debug("Have problems been solved?");

    return 0;
}  // function main

void Quit() {
    flag = false;
}

void GetFPS(float speed) {
    LogSystem::Debug("FPS: {}", speed);
}
