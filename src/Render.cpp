//
// Copyright 2015 infinideas
//

#include "../include/global.hpp"

#include "../include/Render.hpp"

#include "../include/LogSystem.hpp"

#include <stdexcept>

#include <GL/glew.h>

using namespace std;

Window gameWindow;
GLContext oglContext;
TickCounter fpsCounter;

bool RenderPrepare() {
    LogSystem::SetThreadName("Render Thread");

    LogSystem::Info("Preparing to render...");

    LogSystem::Info("Initializing native support...");
    if (!InitNativeSupport()) {
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
        gameWindow = Window(
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
        oglContext = GLContext(gameWindow);
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

    return true;
}


void Rendering() {
    // 渲染循环（临时）
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    oglContext.SwapWindow();
    fpsCounter.Tick();
}


void RenderCleanup() {
    LogSystem::Info("Render thread cleaning up...");

    fpsCounter.StopRaiseEvent();

    oglContext.~GLContext();
    gameWindow.~Window();
    DestroyNativeSupport();
}
