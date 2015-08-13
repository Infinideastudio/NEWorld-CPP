//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_UPDATE_HPP
#define NEWORLD_UPDATE_HPP

#include "TickCounter.hpp"

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

extern TimePoint startTime;
extern float passedTime;
extern TickCounter tpsCounter;

bool UpdatePrepare();
void Updating();
void UpdateCleanup();

void UpdateTime();

#endif  // NEWORLD_UPDATE_HPP
