//
// Copyright 2015 infinideas
//

#include "../include/Render.hpp"

#include <GL/glew.h>

void Rendering() {
    // 渲染循环（临时）
    // glViewport(0, 0, wndWidth, wndHeight);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    context.SwapWindow();
    fpsCounter.Tick();
}
