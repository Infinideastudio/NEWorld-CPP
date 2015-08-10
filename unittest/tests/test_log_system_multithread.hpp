//
// Copyright 2015 infinideas
//

#include "../../include/LogSystem.hpp"

#include <thread>
#include <vector>

#define NUMBER_OF_LOG 100
#define NUMBER_OF_THREAD 8

void test_log_system_multithread();

void WriteLog(int id);

void test_log_system_multithread() {
    using namespace std;

    LogSystem::SetLogDirectory("../temp/unittest/");
    LogSystem::SetRegionName("Unit test");
    LogSystem::SetOutput(true, false);

    vector<thread> threads;

    for (int i = 0;
            i != NUMBER_OF_THREAD;
            ++i) {
        threads.push_back(thread(WriteLog, i));
    }  // for


    for (auto &t : threads) {
        t.join();
    }  // foreach in threads

    LogSystem::CopyToLatest();

}

void WriteLog(int id) {
    LogSystem::SetThreadName(fmt::format("Thread #{}", id));

    for (int i = 0;
            i != NUMBER_OF_LOG;
            ++i) {
        LogSystem::Info("Thread #{} sends this message.", id);
    }  // for
}
