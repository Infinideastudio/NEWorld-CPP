//
// Copyright 2015 infinideas
//

#include "../include/EventsImpl.hpp"
#include "../include/LogSystem.hpp"
#include "../include/Render.hpp"
#include "../include/EventSystem.hpp"
#include "../include/global.hpp"

#include <GL/glew.h>

void Connects() {
    EventSystem::Connect(Events::ApplicationQuit, Quit);
    EventSystem::Connect(Events::WindowResize, ResizeWindow);
    EventSystem::Connect(Events::FPSReport, GetFPS);
    EventSystem::Connect(Events::TPSReport, GetTPS);
}


void Quit() {
    flag = false;
}


void GetFPS(float speed) {
    LogSystem::Debug("FPS: {}", speed);
}


void GetTPS(float speed) {
    LogSystem::Debug("TPS: {}", speed);
}


void ResizeWindow(int /*width*/, int /*height*/) {
    // LogSystem::Debug("Resize window: {}, {}", width, height);

    // wndWidth = width;
    // wndHeight = height;
}
