//#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>

using std::string;
using std::vector;
using std::thread;
using std::mutex;
using std::shared_ptr;
#include "..\Project\GLs\glew.h"
#include "..\Project\GLs\glfw3.h"
#include "blocks.h"
//Types/constants define;
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
typedef unsigned char blockinfo;
typedef unsigned char brightness;

//const double PI = 3.1415926535898;

extern uint BlockTexture[20];
extern uint BlockTextures;
extern uint guiImage[6];
extern uint DestroyImage[11];

extern GLFWwindow* win;

#ifndef _DEF_FUNC_VAR_
#define _DEF_FUNC_VAR_
const uint32 VERSION = 35;
const string MAJOR_VERSION = "Alpha_0.";
const string MINOR_VERSION = "4.9 ";
const string EXT_VERSION = " [Preview Not Release(C++ Edition)]";

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
inline void MutexLock(mutex &mt){
	mt.lock();
}
inline void MutexUnlock(mutex &mt){
	mt.unlock();
}
inline int RoundInt(double d){
	bool n = d < 0;
	if (!n) return int(d + 0.5);
	else return -(int(-d + 0.5));
}
struct ItemPair{
	int ID;
	int amount;
};
#endif