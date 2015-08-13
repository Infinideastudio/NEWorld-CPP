//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_RENDER_HPP
#define NEWORLD_RENDER_HPP

#include "NativeSupport.hpp"
#include "TickCounter.hpp"

extern NativeHandler native;
extern Window window;
extern GLContext context;
extern TickCounter fpsCounter;
// extern int wndWidth, wndHeight;

bool RenderPrepare();
void Rendering();
void RenderCleanup();

#endif  // NEWORLD_RENDER_HPP
