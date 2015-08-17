//
// Copyright 2015 infinideas
//

#include "../include/EventsImpl.hpp"
#include "../include/logging/LogSystem.hpp"
#include "../include/events/EventSystem.hpp"
#include "../include/Render.hpp"
#include "../include/global.hpp"

#include <GL/glew.h>

void ConnectEvents() {
    ConnectEvent(ApplicationQuitEvent, Quit);
    ConnectEvent(WindowResizeEvent, ResizeWindow);
    ConnectEvent(FPSReportEvent, GetFPS);
    ConnectEvent(TPSReportEvent, GetTPS);
}

void Quit() { flag = false; }

void GetFPS(float speed) { LogSystem::Debug("FPS: {}", speed); }

void GetTPS(float speed) { LogSystem::Debug("TPS: {}", speed); }

void ResizeWindow(int /*width*/, int /*height*/) {
    // LogSystem::Debug("Resize window: {}, {}", width, height);

    // wndWidth = width;
    // wndHeight = height;
}
