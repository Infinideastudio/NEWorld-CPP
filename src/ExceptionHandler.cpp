//
// Copyright 2015 infinideas
//

#include "../include/ExceptionHandler.hpp"

#include "../include/LogSystem.hpp"

#include <exception>

using namespace std;

void SetExcetopnHandler() { set_terminate(Terminatehandler); }

void Terminatehandler() {
    LogSystem::Warning("std::terminate() called!");

    try {
        rethrow_exception(current_exception());
    } catch (const exception &e) {
        LogSystem::Fatal(
            "Exception details:\n"
            "e.what() == {}",
            e.what());
    } catch (...) { LogSystem::Fatal("unknown exception catch in terminate."); }
}
