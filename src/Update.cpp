//
// Copyright 2015 infinideas
//

#include "../include/Update.hpp"

#include "../include/global.hpp"
#include "../include/logging/LogSystem.hpp"

#include <thread>

using namespace std;

TimePoint startTime;
float passedTime;
TickCounter tpsCounter;

bool UpdatePrepare() {
    LogSystem::SetThreadName("Update Thread");
    LogSystem::Info("Preparing update thread...");

    startTime = Clock::now();

    tpsCounter.SetCounterTyoe(CounterType::TPS);
    tpsCounter.SetInterval(5000);
    tpsCounter.StartRaiseEvent();

    LogSystem::Info("Update thread done!");
    return true;
}

void UpdateCleanup() {
    LogSystem::Info("Update thread clean up...");

    tpsCounter.StopRaiseEvent();

    LogSystem::Info("Update thread cleaned up!");
}

void UpdateTime() { passedTime = chrono::duration_cast<chrono::duration<float>>(Clock::now() - startTime).count(); }
