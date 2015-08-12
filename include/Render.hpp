//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_RENDER_HPP
#define NEWORLD_RENDER_HPP

#include "NativeSupport.hpp"
#include "TickCounter.hpp"

extern Window gameWindow;
extern GLContext oglContext;
extern TickCounter fpsCounter;

bool RenderPrepare();
void Rendering();
void RenderCleanup();

#endif  // NEWORLD_RENDER_HPP
