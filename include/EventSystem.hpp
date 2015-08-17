//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_EVENT_SYSTEM_HPP

#include "NativeSupport.hpp"
#include "LogSystem.hpp"

#include <atomic>

#include <SDL2/SDL.h>
#include <boost/signals2.hpp>

#define EventType boost::signals2::signal

namespace Events {
[[deprecated]] extern EventType<void()> ApplicationQuit;
[[deprecated]] extern EventType<void(int, int)> WindowResize;
[[deprecated]] extern EventType<void(float)> FPSReport;
[[deprecated]] extern EventType<void(float)> TPSReport;
}  // namespace Events

extern EventType<void()> ApplicationQuitEvent;
extern EventType<void(int, int)> WindowResizeEvent;
extern EventType<void(float)> FPSReportEvent;
extern EventType<void(float)> TPSReportEvent;

void DoEvents();

template <typename SignalType, typename SlotType>
void ConnectEvent(SignalType &signal, SlotType &slot);

template <typename SignalType, typename SlotType>
void ConnectEventAtBack(SignalType &signal, SlotType &slot);

template <typename SignalType, typename SlotType>
void ConnectEventAtFront(SignalType &signal, SlotType &slot);

template <typename SignalType, typename SlotType>
void DisconnectEvent(SignalType &signal, SlotType &slot);

template <typename SignalType>
void DisconnectAllEvent(SignalType &signal);

template <typename SignalType, typename... Args>
void RaiseEvent(SignalType &signal, const Args &... args);

class EventSystem final {
 public:
    // This is a singleton:
    EventSystem() = delete;
    ~EventSystem() = delete;
    EventSystem(const EventSystem &) = delete;
    EventSystem(EventSystem &&) = delete;
    EventSystem &operator=(const EventSystem &) = delete;
    EventSystem &operator=(EventSystem &&) = delete;

    [[deprecated]] static void StartEventSystem();
    [[deprecated]] static void StopEventSystem();

    [[deprecated]] static void EventLoop();

    [[deprecated]] static void DoEvents();

    template <typename SignalType, typename SlotType>
    [[deprecated]] static void Connect(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    [[deprecated]] static void ConnectAtBack(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    [[deprecated]] static void ConnectAtFront(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    [[deprecated]] static void Disconnect(SignalType &signal, SlotType &slot);

    template <typename SignalType>
    [[deprecated]] static void DisconnectAll(SignalType &signal);

    template <typename SignalType, typename... Args>
    [[deprecated]] static void RaiseEvent(SignalType &signal, const Args &... args);

 private:
    [[deprecated]] static std::atomic<bool> m_flag;
};  // singleton class EventSystem

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

template <typename SignalType, typename SlotType>
void ConnectEvent(SignalType &signal, SlotType &slot) {
    signal.connect(slot);
}

template <typename SignalType, typename SlotType>
void ConnectEventAtBack(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_back);
}

template <typename SignalType, typename SlotType>
void ConnectEventAtFront(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_front);
}

template <typename SignalType, typename SlotType>
void DisconnectEvent(SignalType &signal, SlotType &slot) {
    signal.disconnect(slot);
}

template <typename SignalType>
void DisconnectEventAll(SignalType &signal) {
    signal.disconnect_all_slots();
}

#endif
// IFNDEF NEWORLD_EVENT_SYSTEM_HPP
