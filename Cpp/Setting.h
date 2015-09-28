#pragma once

const int defaultwindowwidth = 852;
const int defaultwindowheight = 480;
const int cloudwidth = 32;		//云的宽度

extern int windowwidth;
extern int windowheight;
extern int handleLimit;
extern bool retina;				//retina
extern bool useLoading;         //加载界面
extern bool SmoothLighting;     //平滑光照
extern bool versync;			//垂直同步
extern float FOVyNormal;		//视野角度
extern float FOVyRunning;
extern float FOVyExt;
extern float mousemove;			//鼠标灵敏度
extern int viewdistance;		//视野距离
extern int selectPrecision;		//选择方块的精度
extern int selectDistance;		//选择方块的距离
extern float walkspeed;			//玩家前进速度
extern float runspeed;			//玩家跑步速度
extern int MaxAirJumps;			//空中N段连跳
extern bool UseCIArray;			//是否使用CIA
extern int linelength;			//线的宽度
extern int linedist;
extern float skycolorR;			//天空的颜色
extern float skycolorG;
extern float skycolorB;

