#include "Def.h"

double lastupdate, updateTimer;
bool updateThreadRun, updateThreadPaused;
mutex Mutex;
thread updateThread;
HANDLE hMutex;
GLFWwindow* win;