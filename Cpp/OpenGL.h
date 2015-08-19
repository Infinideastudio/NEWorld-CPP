#pragma once
#include "Def.h"

//OpenGL
extern int GLVersionMajor, GLVersionMinor, GLVersionRev;

void CenterScreen();
void FullScreen();
void setupscreen();
void InitGL();
void setupNormalFog();
void setupWaterFog();
void scrollEvent(GLFWwindow* win, double offsetx, double offsety);
void CharInputFun(GLFWwindow * win, unsigned int c);
void winsizecall(GLFWwindow* window, int width, int height);
void CurPosCall(GLFWwindow*, double x, double y);