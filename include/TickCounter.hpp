//
// Copyright 2015 infinideas
//

#include <chrono>
#include <thread>
#include <atomic>

#ifndef NEWORLD_TICK_COUNTER_HPP
#define NEWORLD_TICK_COUNTER_HPP

typedef std::chrono::milliseconds MillisecondType;
typedef std::chrono::high_resolution_clock ClockType;
typedef std::chrono::time_point<ClockType> TimePointType;

// 默认为5000毫秒
constexpr MillisecondType DefaultInterval = MillisecondType(5000);

enum class CounterType {
    FPS, TPS
};  // enum class CounterType


class TickCounter final {
public:
    TickCounter();
    TickCounter(unsigned interval, CounterType type = CounterType::FPS, bool raiseEvent = true);
    ~TickCounter() = default;

    void Tick();

    void SetCounterTyoe(CounterType type);
    void SetInterval(unsigned interval);
    void StartRaiseEvent();
    void StopRaiseEvent();

    CounterType GetCounterTyoe() const;
    unsigned GetInterval() const;
    bool IsRaiseEvent() const;

    float GetSpeed() const;
private:
    friend void _ThreadLoop(TickCounter *counter);
    CounterType m_counterType;
    bool m_raiseEvent;
    std::atomic<unsigned> m_ticked;
    unsigned m_lastTicked;
    MillisecondType m_interval;
    TimePointType m_lastTime;
    std::thread m_eventThread;
};  // class TickCounter

#endif  // NEWORLD_TICK_COUNTER_HPP
