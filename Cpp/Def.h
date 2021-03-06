//#pragma once
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <set>

using std::string;
using std::vector;
using std::pair;
using std::set;
using std::thread;
using std::mutex;
using std::shared_ptr;
using std::cout;
using std::endl;

#include "..\Project\GLs\glew.h"
#include "..\Project\GLs\glfw3.h"
#include "Setting.h"
#include "Control.h"
#include "blocks.h"
//Types/constants define
typedef signed char sbyte;
typedef unsigned char ubyte;
typedef signed char int8;
typedef short int16;
typedef long int32;
typedef long int int64;
typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;
typedef unsigned char blockprop;
typedef unsigned char brightness;
typedef GLuint TextureID;
//const double PI = 3.1415926535898

extern uint BlockTexture[20];
extern uint BlockTextures;
extern uint guiImage[6];
extern uint DestroyImage[11];

extern GLFWwindow* win;

extern double lastupdate, updateTimer;
extern bool updateThreadRun, updateThreadPaused;
extern mutex Mutex;
extern HANDLE hMutex;
extern thread updateThread;

#ifndef _DEF_FUNC_VAR_
#define _DEF_FUNC_VAR_
const uint32 VERSION = 36;
const string MAJOR_VERSION = "Alpha_0.";
const string MINOR_VERSION = "4.9 ";
const string EXT_VERSION = "";

inline string boolstr(bool b){
	return b ? "True" : "False";
}
inline double rnd(){
	return (double)rand() / RAND_MAX;
}
inline double timer(){
	return (double)clock() / CLOCKS_PER_SEC;
}
inline int sgn(int n){
	if (n > 0) return 1;
	else return n == 0 ? 0 : -1;
}
inline void MutexLock(){
	//mt.lock();
	WaitForSingleObject(hMutex, INFINITE);
}
inline void MutexUnlock(){
	//mt.unlock();
	ReleaseMutex(hMutex);
}
inline int GoInt(double d) {
	if (d >= 0) return int(d);
	else return -(int(-d));
}
inline int RoundInt(float d) {
	if (d >= 0) return int(d + 0.5);
	else return -(int(-d + 0.5));
}
inline int RoundInt(double d){
	if (d >= 0) return int(d + 0.5);
	else return -(int(-d + 0.5));
}
inline string itos(int i){
	char a[12];
	_itoa_s(i, a, 12, 10);
	return string(a);
}
struct ItemPair{
	int ID;
	int amount;
};
#endif