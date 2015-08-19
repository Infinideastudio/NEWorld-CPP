#include "Setting.h"

int windowwidth;
int windowheight;
int handleLimit = 2;
bool retina = false;			//retina
bool useLoading = true;         //加载界面
bool SmoothLighting = true;		//平滑光照
bool versync = false;			//垂直同步
float FOVyNormal = 60.0;		//视野角度
float FOVyRunning = 8.0f;
float FOVyExt;
float mousemove = 0.25;			//鼠标灵敏度
int viewdistance = 5;			//视野距离
int selectPrecision = 100;		//选择方块的精度
int selectDistance = 10;		//选择方块的距离
float walkspeed = 0.15f;		//玩家前进速度
float runspeed = 0.3f;			//玩家跑步速度
int MaxAirJumps = 2;			//空中N段连跳
bool UseCIArray = false;		//使用CIA
int linelength = 10;
int linedist = 30;
float skycolorR = 0.7f;
float skycolorG = 1.0f;
float skycolorB = 1.0f;