//
// Copyright 2015 infinideas
//

#include "../include/LogSystem.hpp"
#include "../include/NativeSupport.hpp"
#include "../include/EventSystem.hpp"
#include "../include/TickCounter.hpp"
#include "../include/Tools.hpp"

#include <cstdlib>
#include <atomic>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

static atomic<bool> flag(true);

void Quit();
void GetFPS(float speed);

int main(/*int argc, char *argv[]*/) {
    // 载入日志
    LogSystem::SetLogDirectory(GetCurrentDirectory() + "logs/");
    LogSystem::SetRegionName("Client");
    LogSystem::SetThreadName("Main Thread");
    LogSystem::SetOutput(true, true);

    LogSystem::Info("Log system started.");
    atexit(LogSystem::CopyToLatest);

    // 载入SDL2
    LogSystem::Info("Loading SDL2...");
    InitNativeSupport();
    atexit(DestroyNativeSupport);

    // 创建窗口
    LogSystem::Info("Creating window...");
    Window wnd("NEWorld 0.0.1",
               WINDOWPOS_CENTERED, WINDOWPOS_CENTERED,
               800, 600, WindowFlags::OpenGL
              );

    // 准备OpenGL上下文
    LogSystem::Info("Setting up OpenGL context...");
    GLContext::SetOpenGLVersion(3, 3);
    GLContext context(wnd);
    // context.MakeCurrent();

    // 绑定事件
    LogSystem::Info("Connecting signals with slots...");
    EventSystem::StartEventSystem();
    EventSystem::Connect(Events::ApplicationQuit, Quit);
    EventSystem::Connect(Events::FPSReport, GetFPS);
    atexit(EventSystem::StopEventSystem);

    // 主循环（临时）
    LogSystem::Info("Start main loop.");

    TickCounter fpsc;
    while (flag) {
        EventSystem::DoEvents();

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        context.SwapWindow();
        fpsc.Tick();
    }   // while

    LogSystem::Info("Exited main loop.");

    return 0;
}  // function main

void Quit() {
    flag = false;
}


void GetFPS(float speed) {
    LogSystem::Debug("FPS: {}", speed);
}
