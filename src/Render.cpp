//
// Copyright 2015 infinideas
//

#include "../include/global.hpp"

#include "../include/Render.hpp"

#include "../include/LogSystem.hpp"

#include <stdexcept>

#include <GL/glew.h>

using namespace std;

NativeHandler native;
Window window;
GLContext context;
TickCounter fpsCounter;
// int wndWidth = 800, wndHeight = 600;

bool RenderPrepare() {
    LogSystem::SetThreadName("Render Thread");

    LogSystem::Info("Preparing render thread...");

    LogSystem::Info("Initializing native support...");
    if (!native.IsVaild()) {
        LogSystem::Fatal(
            "Cannot init native support!\n"
            "Details: {}",
            GetError()
        );
        return false;
    } else {
        LogSystem::Info("Native support initialization is OK!");
    }

    LogSystem::Info("Creating game window...");
    try {
        window = Window(
                     "NEWorld",
                     WINDOWPOS_CENTERED, WINDOWPOS_CENTERED,
                     800, 600,
                     WindowFlags::OpenGL
                 );
    } catch (const runtime_error &e) {
        LogSystem::Fatal(
            "Cannot create game window!\n"
            "Catched exception: {}\n"
            "Details: {}",
            e.what(),
            GetError()
        );
        return false;
    }
    LogSystem::Info("Game window created successfully!");

    LogSystem::Info("initializing GLEW...");
    glewExperimental = true;
    glewInit();
    glGetError();  // GLEW载入后会设置一个没用的错误，忽略即可
    LogSystem::Info("GLEW inited!");

    LogSystem::Info("Creating OpengGL context...");
    if (GLContext::SetOpenGLVersion(3, 3) < SUCCESS) {
        LogSystem::Error(
            "Cannot set OpenGL version to 3.3.\n"
            "Details: {}",
            GetError()
        );
    }
    try {
        context = GLContext(window);
    } catch (const runtime_error &e) {
        LogSystem::Fatal(
            "Cannot create OpenGL context!\n"
            "Catched exception: {}\n"
            "Details: {}",
            e.what(),
            GetError()
        );
        return false;
    }
    LogSystem::Info("OpenGL context created successfully!");

    LogSystem::Info("Preparing other things...");
    fpsCounter.SetCounterTyoe(CounterType::FPS);
    fpsCounter.SetInterval(5000);
    fpsCounter.StartRaiseEvent();

    LogSystem::Info("Render thread done!");

    return true;
}


void RenderCleanup() {
    LogSystem::Info("Render thread cleaning up...");

    fpsCounter.StopRaiseEvent();

    LogSystem::Debug("Resources will be destructed after main().");
    // context.~GLContext();
    // window.~Window();
    // native.~NativeHandler();

    LogSystem::Info("Render thread cleaned up!");
}
