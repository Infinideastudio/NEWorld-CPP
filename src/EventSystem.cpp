//
// Copyright 2015 infinideas
//

#include "../include/EventSystem.hpp"

#include "../include/LogSystem.hpp"
#include "../include/NativeSupport.hpp"

using namespace std;

atomic<bool> EventSystem::m_flag;

namespace Events {
EventType<void()> ApplicationQuit;
EventType<void(int, int)> WindowResize;
EventType<void(float)> FPSReport;
EventType<void(float)> TPSReport;
}  // namespace Events

void EventSystem::StartEventSystem() { m_flag = true; }

void EventSystem::StopEventSystem() { m_flag = false; }

void EventSystem::EventLoop() {
    while (m_flag) { DoEvents(); }  // while
}

void EventSystem::DoEvents() {
    NativeEventType event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                LogSystem::Debug("Event System: Recevied SDL_QUIT.");
                Events::ApplicationQuit();
                break;

            case SDL_WINDOWEVENT:
                // LogSystem::Debug("Event System: Received SDL_WINDOWEVENT.");

                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        // LogSystem::Debug("Event System: Received SDL_WINDOW_RESIZED.");

                        Events::WindowResize(event.window.data1, event.window.data2);
                        break;
                }  // switch to event.window.event

                break;

        }  // switch to event.type

    }  // while
}
