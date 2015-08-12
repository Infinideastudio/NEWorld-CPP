//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_EVENT_SYSTEM_HPP

#include <atomic>

#include <SDL2/SDL.h>
#include <boost/signals2.hpp>

namespace Events {
#define EventType boost::signals2::signal

typedef EventType<void()> ApplicationQuitEvent;
typedef EventType<void(float)> FPSReportEvent;
typedef EventType<void(float)> TPSReportEvent;

extern ApplicationQuitEvent ApplicationQuit;
extern FPSReportEvent FPSReport;
extern TPSReportEvent TPSReport;
}  // namespace Events

class EventSystem final {
public:
    //This is a singleton:
    EventSystem() = delete;
    ~EventSystem() = delete;
    EventSystem(const EventSystem&) = delete;
    EventSystem(EventSystem&&) = delete;
    EventSystem &operator=(const EventSystem&) = delete;
    EventSystem &operator=(EventSystem&&) = delete;

    static void StartEventSystem();
    static void StopEventSystem();

    static void EventLoop();
    static void DoEvents();

    template <typename SignalType, typename SlotType>
    static void Connect(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    static void ConnectAtBack(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    static void ConnectAtFront(SignalType &signal, SlotType &slot);

    template <typename SignalType, typename SlotType>
    static void Disconnect(SignalType &signal, SlotType &slot);

    template <typename SignalType>
    static void DisconnectAll(SignalType &signal);

    template <typename SignalType, typename ... Args>
    static void RaiseEvent(SignalType &signal, const Args & ... args);

private:
    static std::atomic<bool> m_flag;
};  // singleton class EventSystem


template <typename SignalType, typename SlotType>
void EventSystem::Connect(SignalType &signal, SlotType &slot) {
    signal.connect(slot);
}


template <typename SignalType, typename SlotType>
void EventSystem::ConnectAtBack(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_back);
}


template <typename SignalType, typename SlotType>
void EventSystem::ConnectAtFront(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_front);
}


template <typename SignalType, typename SlotType>
void EventSystem::Disconnect(SignalType &signal, SlotType &slot) {
    signal.disconnect(slot);
}


template <typename SignalType>
void EventSystem::DisconnectAll(SignalType &signal) {
    signal.disconnect_all_slots();
}


template <typename SignalType, typename ... Args>
void EventSystem::RaiseEvent(SignalType &signal, const Args & ... args) {
    signal(args...);
}


#endif
// IFNDEF NEWORLD_EVENT_SYSTEM_HPP
