//
// Copyright 2015 infinideas
//

#include "../include/TickCounter.hpp"

#include "../include/EventSystem.hpp"

using namespace std;
using namespace std::chrono;


void _ThreadLoop(TickCounter *counter) {
    while (counter->m_raiseEvent) {
        this_thread::sleep_for(counter->m_interval);
        float speed = counter->m_ticked / static_cast<float>(counter->m_interval.count()) * 1000.0f;

        switch (counter->m_counterType) {
        case CounterType::FPS:
            EventSystem::RaiseEvent(Events::FPSReport, speed);
            break;

        case CounterType::TPS:
            EventSystem::RaiseEvent(Events::TPSReport, speed);
            break;
        }  // switch to counter->m_counterType


        counter->m_lastTicked = counter->m_ticked;
        counter->m_ticked = 0;
        counter->m_lastTime = ClockType::now();
    }
}

TickCounter::TickCounter()
    : m_counterType(CounterType::FPS),
      m_raiseEvent(true),
      m_ticked(0),
      m_lastTicked(0),
      m_interval(DefaultInterval),
      m_lastTime(ClockType::now())
{
    m_eventThread = thread(_ThreadLoop, this);
    m_eventThread.detach();
}


TickCounter::TickCounter(unsigned interval, CounterType type, bool raiseEvent)
    : m_counterType(type),
      m_raiseEvent(raiseEvent),
      m_ticked(0),
      m_lastTicked(0),
      m_interval(interval),
      m_lastTime(ClockType::now())
{
    if (raiseEvent) {
        m_eventThread = thread(_ThreadLoop, this);
        m_eventThread.detach();
    }
}


void TickCounter::Tick() {
    ++m_ticked;
}


void TickCounter::SetCounterTyoe(CounterType type) {
    m_counterType = type;
}


void TickCounter::SetInterval(unsigned interval) {
    m_interval = MillisecondType(interval);
}


void TickCounter::SetRaiseEvent(bool value) {
    // 防止重新启动
    if (value && !m_raiseEvent) {
        m_eventThread = thread(_ThreadLoop, this);
        m_eventThread.detach();
    }

    m_raiseEvent = value;
}


float TickCounter::GetSpeed() const {
    return m_lastTicked / static_cast<float>(m_interval.count()) * 1000.0f;
}
