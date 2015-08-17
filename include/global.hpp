//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_GLOBAL_HPP
#define NEWORLD_GLOBAL_HPP

#include "../include/Thread.hpp"

#include <string>
#include <atomic>

constexpr unsigned EVENT_THREAD_SLEEP_FOR = 10;
constexpr unsigned UPDATE_THREAD_SLEEP_FOR = 20;

extern std::string VERSION;

extern int WindowWidth, WindowHeight;

extern std::atomic<bool> flag;

#endif  // NEWORLD_GLOBAL_HPP
