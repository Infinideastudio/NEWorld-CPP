//
// Copyright 2015 infinideas
//

#include "../include/EventSystem.hpp"

#include "../include/LogSystem.hpp"
#include "../include/NativeSupport.hpp"

using namespace std;

atomic<bool> EventSystem::m_flag;

namespace Events {
ApplicationQuitEvent ApplicationQuit;
FPSReportEvent FPSReport;
TPSReportEvent TPSReport;
}  // namespace Events

void EventSystem::StartEventSystem() {
    m_flag = true;
}


void EventSystem::StopEventSystem() {
    m_flag = false;
}


void EventSystem::EventLoop() {
    while (m_flag) {
        DoEvents();
    }   // while
}


void EventSystem::DoEvents() {
    SDLEventType event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_QUIT:
            LogSystem::Debug("Event System: Recevied SDL_QUIT.");
            Events::ApplicationQuit();
            break;

        }  // switch to event.type

    }   // while
}
