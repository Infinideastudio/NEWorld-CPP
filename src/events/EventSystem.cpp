//
// Copyright 2015 infinideas
//

#include "../../include/events/EventSystem.hpp"

EventType<void()> ApplicationQuitEvent;
EventType<void(int, int)> WindowResizeEvent;
EventType<void(float)> FPSReportEvent;
EventType<void(float)> TPSReportEvent;

void DoEvents() {
    NativeEventType event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                LogSystem::Debug("Event System: Recevied SDL_QUIT.");
                ApplicationQuitEvent();
                break;

            case SDL_WINDOWEVENT:
                // LogSystem::Debug("Event System: Received SDL_WINDOWEVENT.");

                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        // LogSystem::Debug("Event System: Received SDL_WINDOW_RESIZED.");

                        WindowResizeEvent(event.window.data1, event.window.data2);
                        break;
                }  // switch to event.window.event

                break;

        }  // switch to event.type

    }  // while
}
