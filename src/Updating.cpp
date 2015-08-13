//
// Copyright 2015 infinideas
//

#include "../include/Update.hpp"

#include "../include/global.hpp"

#include <thread>

using namespace std;

void Updating() {
    UpdateTime();
    // ====================== //

    // ====================== //
    tpsCounter.Tick();
    this_thread::sleep_for(chrono::milliseconds(UPDATE_THREAD_SLEEP_FOR));
}
