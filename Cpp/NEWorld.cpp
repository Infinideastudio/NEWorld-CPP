//====================<----------NEWorld-NE世界---------->======================//
//|开发者：乔展荣[qiaozhanrong]                                                |//
//|Developed by Qiao Zhanrong                                                 |//
//|官方网站：http://www.neworldsite.icoc.cc/                                   |//
//|Official website http://www.neworldsite.icoc.cc/                           |//
//|百度贴吧：http://tieba.baidu.com/p/2822071396?pn=1                          |//
//|BaiduTieba topic address http://tieba.baidu.com/p/2822071396?pn=1          |//
//|地址：中国-江苏-南京-玄武区-四牌楼-文昌街2号                                   |//
//|Wenchang street No.2,Nanjing,Jiangsu Province,P.R.C.                        |//
//|地址2：中国-江苏-南京-玄武区-南京师范大学附属小学[四牌楼校区]-4楼六(4)班        |//
//|Class 4, Grade 6, The Primary School Attached To Nanjing Normal University |//
//|渣翻译不解释QAQ ( My English is not very good ... Don't mind ! )            |//
//|----------------------------------------------------------------------------|//
//|NEWolrd原版   语言：FreeBASIC          编译器版本：FBC 1.03.0 及以上          |//
//|Programming language FreeBASIC(Compiler Version: 1.03.0+)                  |//;
//|NEWorld-CPP版 语言：C++                编译器版本：VC++ with C++11           |//
//|Programming language C++(Compiler Version; VC++ with C++11)                |//;
//|版本：0.4.9 [Alpha]                                                        |//
//|Game version: Alpha 0.4.9                                                   |//
//==============================================================================//
//向Minecraft作者Markus Persson致敬！
//THANKS A LOT to the author Markus Persson Minecraft!
//为中国的IT事业作出贡献！
//Make a contribution to the China IT career!
//==============================================================================//
//
//
//==============================   Initialize   ================================//
//==============================初始化(包括闪屏)================================//

/*

NEWorld-CPP第一版作者（翻译作者）：Null (abc612008)

更新记录：
0.4.9_Preview_0.0.1 & 0.0.2
1. 以0.4.9_Preview_150414为基础翻译
2. 将glfw2更换成glfw3
3. 将pblocks,pbrightness换成数组

0.4.9_Preview_0.0.3 & 0.0.4(未发布)
1. 更新版本至0.4.9_Preview_150713
2. 更换chunk保存格式（与FB版地图不再兼容）
3. 背包数组从0用起
4. 更换背包数组保存格式（与FB版地图不再兼容）
5. 优化了renderer::init,world::getblock等函数
6. Frustum优化
7. 其他各种优化
8. 修复了拾取问题
9. 修复了chunk加载范围问题
10. 选项界面完成，进入游戏的界面有BUG

0.4.9_Preview_0.0.5
1. 更新版本到v35(内测版)
2. 解决了/W4级别下的绝大多数警告
3. 各种优化

0.4.9_Preview_0.0.6
1. 重构方块属性
2. 继续优化
3. 调整了控制台的换行
4. 删除了pickblock等函数的控制台输出
5. 去掉了注释行末的分号

*/

#include <Windows.h> //只是要一个Sleep函数而已

#include "DeveloperOptions.h"
#include "Def.h"
#include "blocks.h"
#include "textures.h"
#include "glprinting.h"
#include "renderer.h"
#include "Hitbox.h"
#include "player.h"
#include "WorldGen.h"
#include "world.h"
#include "particles.h"
#include "frustum.h"
#include "menus.h"
#include "Cat.h"
//socket,server,client
//#define MAKE_WORD(a,b) cast(ushort,((a)and &hff)or(cast(ushort,((b)and &hff))shl 8))

const int defaultwindowwidth = 850 * 2;
const int defaultwindowheight = 480 * 2;
bool versync = false;            //垂直同步
float FOVyNormal = 60.0;           //视野角度
float mousemove = 0.25;            //鼠标灵敏度
int viewdistance = 5;          //视野距离
int cloudwidth = 32;       //云的宽度
int selectPrecision = 100;     //选择方块的精度
int selectDistance = 10;     //选择方块的距离
float playerHeight = 1.2f;     //玩家的高度
float walkspeed = 0.15f;       //玩家前进速度
float runspeed = 0.3f;        //玩家跑步速度
int MaxAirJumps = 2;     //空中N段连跳
bool UseCIArray = false;  //使用CIA
int linelength = 10;      //Press F3
int linedist = 30;        //Press F3
float skycolorR = 0.7f;
float skycolorG = 1.0f;
float skycolorB = 1.0f;

float FOVyRunning = 8.0f;
float FOVyExt;
float playerHeightExt = 0.0f;
string inputstr;

int renderedChunk = 0;

//OpenGL
int GLVersionMajor, GLVersionMinor, GLVersionRev;

//线程 [Threads]
double lastupdate, updateTimer;
bool updateThreadRun, updateThreadPaused;
mutex Mutex; //SIGN
thread updateThread;  //SIGN

//多人游戏 [Multiplayer]
bool mpclient, mpserver;

void CenterScreen();
void FullScreen();
void splashscreen();
void outputGameInfo();
void setupscreen();
void InitGL();
void setupNormalFog();
void setupWaterFog();
void LoadTextures();
void loadGame();
void saveGame();
void updateThreadFunc();
void drawcloud(double px, double pz);
void updategame();
void drawmain();
void drawborder(int x, int y, int z);
void renderDestroy(float level, int x, int y, int z);
void Renderoptions();
void bagUpdate();
void additem(block itemname);
void scrollEvent(GLFWwindow* win, double offsetx, double offsety);
void saveScreenshot(int x, int y, int w, int h, string filename);
void createThumbnail();
void CharInputFun(GLFWwindow * win, unsigned int c);

//Variables define

//鼠标输入数据
double mx;
double my;
int mw;
int mb;
int mbp;
int mbl;
int mwl;

int windowwidth;                       //窗口宽度;
int windowheight;                      //窗口高度;
bool gamebegin;
bool bagOpened;

int fps;                               //帧速率;
int fpsc;                              //帧数计数;
double fctime;
int ups;
int upsc;
double uctime;
bool shouldGetScreenshot, shouldGetThumbnail;

float lx, ly, lz, sidedist[7], sidedistmin;  //选择方块的变量;\

GLuint splTex;
GLuint BlockTexture[20];
GLuint BlockTextures;
GLuint guiImage[6];
GLuint DestroyImage[11];

bool FirstUpdateThisFrame;        //这可能是整个程序中最长的一个变量名了。。。;

block BlockInHand = blocks::AIR;
ubyte itemInHand = 0;
block inventorybox[4][10];
block inventorypcs[4][10];

bool EP;
bool ESCP;

bool GUIrenderswitch;
bool DEBUGMODE;                  //调试模式;
bool DebugHitbox;            //Hitbox调试模式;
bool DebugChunk;             //区块调试模式[Not enabled!];

int selx, sely, selz, oldselx, oldsely, oldselz;
float selt, seldes;
block selb;
brightness selbr;
int selbx, selby, selbz, selcx, selcy, selcz;
double Time_overall, Time_renderscene, Time_renderGUI, Time_updategame, Time_screensync;
double Time_overall_, Time_renderscene_, Time_renderGUI_, Time_updategame_, Time_screensync_; //?check

//==============================  Main Program  ================================//
//==============================     主程序     ================================//

//下面请看qiaozhanrong童鞋作死！
int main(){
	//......
	//......
	//......
	//......"顺便说一下，GL有个意思叫女同性恋，OpenGL翻译成中文就是。。。"
	//......
	//......
	//......
	//......"开放式女同性恋(我了个大去啊)!"
	//......
	//......
	//......


	memset(inventorybox, 0, sizeof(inventorybox));
	memset(inventorypcs, 0, sizeof(inventorypcs));
	inventorybox[3][0] = blocks::WOOD;
	inventorypcs[3][0] = 32;
	//========主菜单(就怪了)========

	windowwidth = defaultwindowwidth;
	windowheight = defaultwindowheight;
	std::cout << "[Console][Debug] Init OpenGL... "<<glfwInit() << "/1 ";
	std::stringstream ss;
	ss << "NEWorld " << MAJOR_VERSION << MINOR_VERSION << EXT_VERSION;
	win = glfwCreateWindow(windowwidth, windowheight, ss.str().c_str(), NULL, NULL);
	glfwMakeContextCurrent(win);
	std::cout << glewInit() << "/0" << std::endl;
	CenterScreen();
	glfwSetInputMode(win, GLFW_STICKY_KEYS, true); //?
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(win, &scrollEvent);
	glfwSetCharCallback(win, &CharInputFun);
	//glfwSetWindowSizeCallback(&WindowSizeFun)
	//glfwSetMouseButtonCallback(&mousebuttonfun)
	setupscreen();

	//渐变效果
	glDisable(GL_CULL_FACE);

	splashscreen();

	InitGL();

	LoadTextures();

	//主菜单
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
main_menu:
	gamebegin = false;
	glDisable(GL_LINE_SMOOTH);
	mainmenu();
	glEnable(GL_LINE_SMOOTH);

	/*COLOR_CHANGE_BEFORE*/ printf("[Console][Game]");
	/*COLOR_CHANGE_BEFORE*/ printf("Starting game...\n");
	//游戏开始(还没呢QAQ)
	//各种初始化ing...
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glprint(350, 240 + 16 * 3, ("Loading... "));
	glprint(350, 240 + 16 * 1, ("Initializing!\n"));
	glfwSwapBuffers(win);
	glfwPollEvents();

	//Mutex
	MutexLock(Mutex);
	updateThread = thread(updateThreadFunc);

	//SIGN
	//if (mpclient )  client.clientThread = ThreadCreate(&client.clientThreadFunc(), cast(any ptr, 0))
	//if (mpserver )  server.serverThread = ThreadCreate(&server.serverThreadFunc(), cast(any ptr, 0))

	//初始化游戏状态
	/*COLOR_CHANGE_BEFORE*/ printf("[Console][Game]");
	/*COLOR_CHANGE_BEFORE*/ printf("Init player...\n");
	player::InitHitbox();
	player::xpos = 0;
	player::ypos = 150;
	player::zpos = 0;
	loadGame();
	player::MoveHitboxToPosition();
	player::InitPosition();
	/*COLOR_CHANGE_BEFORE*/ printf("[Console][Game]");
	/*COLOR_CHANGE_BEFORE*/ printf("Init world...\n");
	world::Init();
	GUIrenderswitch = true;
	world::sortChunkRenderList((int)player::xpos, (int)player::ypos, (int)player::zpos);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	fctime = timer(); uctime = timer(); lastupdate = timer();
	setupNormalFog();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glprint(350, 240 + 16 * 3, "Loading...");
	glprint(350, 240 + 16 * 1, "Creating chunks!");
	glfwSwapBuffers(win);
	glfwPollEvents();
	/*COLOR_CHANGE_BEFORE*/ printf("[Console][Game]");
	/*COLOR_CHANGE_BEFORE*/ printf("Game start!\n");

	//这才是游戏开始!
	glClearColor(skycolorR, skycolorG, skycolorB, 1.0);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(win, windowwidth / 2, windowheight / 2);
	/*COLOR_CHANGE_BEFORE*/ printf("[Console][Game]");
	/*COLOR_CHANGE_BEFORE*/ printf("Main loop started\n");
	updateThreadRun = true;
	if (mpserver) glfwSetWindowTitle(win, "NEWorld [Server Mode: debug]");  //偷懒不解释
	if (mpclient) glfwSetWindowTitle(win, "NEWorld [Client Mode: debug]");

	do{
		//主循环，被简化成这样，惨不忍睹啊！
		//MutexUnlock(Mutex)
		//MutexLock(Mutex)
		Time_overall_ = timer();

		if ((timer() - uctime) >= 1.0){
			uctime = timer();
			ups = upsc;
			upsc = 0;
		}

		drawmain();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == 1){
			createThumbnail();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			setFontColor(1.0, 1.0, 1.0, 1.0);
			glprint(0, 0, "Saving world...");
			glfwSwapBuffers(win);
			glfwPollEvents();
			saveGame();
			world::destroyAllChunks();
			updateThreadRun = false;
			MutexUnlock(Mutex);
			updateThread.join();
			printf("[Console][Game]");
			printf("Returned to main menu\n");
			ESCP = true;
			//exit(0)
			goto main_menu;
		}

		Time_overall = timer() - Time_overall;

	} while (!glfwWindowShouldClose(win));
	saveGame();
	updateThreadRun = false;
	MutexUnlock(Mutex);
	updateThread.join();
	//MutexDestroy(Mutex)
	//结束程序，删了也没关系 ←_←（吐槽FB和glfw中）
	glfwTerminate();
	return 0;
}

void CharInputFun(GLFWwindow * win, unsigned int c){
	if (c >= 128){
		wchar_t* pwszUnicode=new wchar_t[2];
		pwszUnicode[0] = c;
		pwszUnicode[1] = '\0';
		int iSize;
		char* pszMultiByte;
		iSize = WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, NULL, 0, NULL, NULL);
		pszMultiByte = (char*)malloc((iSize + 1));
		WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, pszMultiByte, iSize, NULL, NULL);
		inputstr += pszMultiByte;
		free(pszMultiByte);
		delete[] pwszUnicode;
	}
	else{
		inputstr += (char)c;
	}
}

void updateThreadFunc(){
	//游戏更新线程函数

	//Wait until start...
	MutexLock(Mutex);
	while (!updateThreadRun){
		MutexUnlock(Mutex);
		MutexLock(Mutex);
	}
	MutexUnlock(Mutex);

	//Thread starts
	MutexLock(Mutex);
	while (updateThreadRun){

		MutexUnlock(Mutex);
		MutexLock(Mutex);
		while (updateThreadPaused){
			MutexUnlock(Mutex);
			MutexLock(Mutex);
		}

		Time_updategame = 0;
		FirstUpdateThisFrame = true;

		updateTimer = timer();
		while ((updateTimer - lastupdate) >= 1 / (double)30 && upsc < 60){
			lastupdate += 1 / (double)30;
			upsc += 1;
			updategame();
			FirstUpdateThisFrame = false;
		}

		if ((timer() - uctime) >= 1.0) {
			uctime = timer();
			ups = upsc;
			upsc = 0;
		}

	}
	MutexUnlock(Mutex);
}

//glfw callback函数
//  void WindowSizeFun GLFWCALL(int w, int h){
//    windowwidth=w
//    windowheight=iif((h>0,h,1)
//    setupscreen()
//  }
//  
//  void MouseButtonFun GLFWCALL(int button, int action){
//    mb=0
//	if (action = GLFW_PRESS){
//		if (button = GLFW_MOUSE_BUTTON_LEFT) mb++
//		if (button = GLFW_MOUSE_BUTTON_RIGHT) mb += 2
//		if (button = GLFW_MOUSE_BUTTON_MIDDLE) mb += 4
//	}
//}

void scrollEvent(GLFWwindow* win, double offsetx, double offsety){
	//mw = offsety
	itemInHand -= (ubyte)sgn(static_cast<int>(offsety));
	if (itemInHand > 9) itemInHand = 0;
	if (itemInHand < 0) itemInHand = 9;
}

void CenterScreen(){
	//   int w,h
	//GLFWvidmode v
	// w=v.width
	// h=v.height
	// w-=windowwidth
	// h-=windowheight
	// glfwSetWindowPos(w/2,h/2)
}

void FullScreen(){
	//GLFWvidmode v
	//   glfwGetDesktopMode(&v)
	//   windowwidth=v.width
	//   windowheight=v.height
	//   glfwCloseWindow()
	//   glfwOpenWindow(windowwidth,windowheight,0,0,0,0,32,0,GLFW_FULLSCREEN)
	//   setupScreen()
}

void splashscreen(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, windowwidth, windowheight, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	splTex = textures::LoadRGBATexture("textures\\gui\\SplashScreen.bmp", "");

	int i;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, splTex);
	for (i = 0; i != 256; i += 4){
		glfwSwapBuffers(win);
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D, splTex);
		glColor4f(i / (float)256, i / (float)256, i / (float)256, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);												 glVertex2i(0, 0);
		glTexCoord2f(windowwidth / 1024.0f, 1.0f);								 glVertex2i(windowwidth, 0);
		glTexCoord2f(windowwidth / 1024.0f, 1.0f - windowheight / 1024.0f);		 glVertex2i(windowwidth, windowheight);
		glTexCoord2f(0.0f, 1.0f - windowheight / 1024.0f);						 glVertex2i(0, windowheight);
		glEnd();
		setFontColor(0.0, 0.0, 0.0, 1.0);
		//SIGN glprint(400, 200, MAJOR_VERSION & MINOR_VERSION & " [v" & VERSION & "]")
		//glfwSleep(0.001)
		Sleep(1);
	}
	glprint(10, 10, "Loading Textures...");
	glprint(10, 26, "Please Wait...");
	glfwSwapBuffers(win);
}

void outputGameInfo(){

	//color 15,2
	system("cls");
	printf("NEWorld %s%s[console output]\n", MAJOR_VERSION.c_str(), MINOR_VERSION.c_str());
	printf("┌ 新创无际工作室欢迎您的加入！┐            \n ");
	printf("└ QQ群号：434811287          ┘            \n");
	printf("[这都被你发现了。。。]          \n");

	//color 7,0
	//view printf 5 to 23
	//cls

}

void setupscreen(){

	//OpenGL参数设置
	glViewport(0, 0, windowwidth, windowheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glAlphaFunc(GL_GREATER, 0.0); //<--这家伙在卖萌？(往后面看看，卖萌的多着呢);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//这顶点数组真TM的坑爹= =

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	BuildFont(windowwidth, windowheight);
	setFontColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(skycolorR, skycolorG, skycolorB, 1.0);

}

void InitGL(){
	GLVersionMajor = glfwGetWindowAttrib(win, GLFW_CONTEXT_VERSION_MAJOR);
	GLVersionMinor = glfwGetWindowAttrib(win, GLFW_CONTEXT_VERSION_MINOR);
	GLVersionRev = glfwGetWindowAttrib(win, GLFW_CONTEXT_REVISION);

	//glGenBuffers/*ARB*/=glfwGetProcAddress("glGenBuffers/*ARB*/")
	//if( glGenBuffers/*ARB*/=0 ) printf "glGenBuffers/*ARB*/ Not Avaliable."
	//glBindBuffer/*ARB*/=glfwGetProcAddress("glBindBuffer/*ARB*/")
	//if( glBindBuffer/*ARB*/=0 ) printf "glBindBuffer/*ARB*/ Not Avaliable."
	//glBufferData/*ARB*/=glfwGetProcAddress("glBufferData/*ARB*/")
	//if( glBufferData/*ARB*/=0 ) printf "glBufferData/*ARB*/ Not Avaliable."
	//glDeleteBuffers/*ARB*/=glfwGetProcAddress("glDeleteBuffers/*ARB*/")
	//if( glDeleteBuffers/*ARB*/=0 ) printf "glDeleteBuffers/*ARB*/ Not Avaliable."

}

void setupNormalFog(){

	glEnable(GL_FOG);
	uint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
	float fogColor[4] = { skycolorR, skycolorG, skycolorB, 1.0 };
	uint fogfilter = 2;

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, &fogColor[0]);
	//glFogf GL_FOG_DENSITY, 1.0
	glFogf(GL_FOG_START, viewdistance * 16.0f - 32.0f);
	glFogf(GL_FOG_END, viewdistance * 16.0f);


}

void setupDepthFog(int d){

	glEnable(GL_FOG);
	uint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
	float fogColor[4] = { skycolorR, skycolorG, skycolorB, 1.0f };
	uint fogfilter = 2;

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, &fogColor[0]);
	glFogf(GL_FOG_START, float(d - 8));
	glFogf(GL_FOG_END, float(d + 8));

}

void setupWaterFog(){

	glEnable(GL_FOG);
	uint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
	float fogColor[] = { 0.3f, 0.3f, 1.0f, 1.0f };
	uint fogfilter = 1;

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, &fogColor[0]);
	glFogf(GL_FOG_DENSITY, 0.1f);
	glFogf(GL_FOG_START, 240.0f);
	glFogf(GL_FOG_END, 480.0f);

}

void LoadTextures(){
	//载入纹理
	textures::Init();

	BlockTexture[textures::ROCK] = textures::LoadRGBATexture("textures\\blocks\\stone.bmp", "");
	BlockTexture[textures::GRASS_TOP] = textures::LoadRGBATexture("textures\\blocks\\grass_top.bmp", "");
	BlockTexture[textures::GRASS_SIDE] = textures::LoadRGBATexture("textures\\blocks\\grass_side.bmp", "");
	BlockTexture[textures::DIRT] = textures::LoadRGBATexture("textures\\blocks\\dirt.bmp", "");
	BlockTexture[textures::STONE] = textures::LoadRGBATexture("textures\\blocks\\cobblestone.bmp", "");
	BlockTexture[textures::PLANK] = textures::LoadRGBATexture("textures\\blocks\\plank.bmp", "");
	BlockTexture[textures::WOOD_TOP] = textures::LoadRGBATexture("textures\\blocks\\wood_top.bmp", "");
	BlockTexture[textures::WOOD_SIDE] = textures::LoadRGBATexture("textures\\blocks\\wood_side.bmp", "");
	BlockTexture[textures::BEDROCK] = textures::LoadRGBATexture("textures\\blocks\\bedrock.bmp", "");
	BlockTexture[textures::LEAF] = textures::LoadRGBATexture("textures\\blocks\\leaf.bmp", "textures\\blocks\\leafmask.bmp");
	BlockTexture[textures::GLASS] = textures::LoadRGBATexture("textures\\blocks\\glass.bmp", "textures\\blocks\\glassmask.bmp");
	BlockTexture[textures::WATER] = textures::LoadRGBATexture("textures\\blocks\\water.bmp", "textures\\blocks\\watermask.bmp");
	BlockTexture[textures::LAVA] = textures::LoadRGBATexture("textures\\blocks\\lava.bmp", "textures\\blocks\\lavamask.bmp");
	BlockTexture[textures::GLOWSTONE] = textures::LoadRGBATexture("textures\\blocks\\glowstone.bmp", "");
	BlockTexture[textures::SAND] = textures::LoadRGBATexture("textures\\blocks\\sand.bmp", "");
	BlockTexture[textures::CEMENT] = textures::LoadRGBATexture("textures\\blocks\\cement.bmp", "");
	BlockTexture[textures::ICE] = textures::LoadRGBATexture("textures\\blocks\\ice.bmp", "textures\\blocks\\icemask.bmp");
	BlockTexture[textures::COAL] = textures::LoadRGBATexture("textures\\blocks\\coal.bmp", "");
	BlockTexture[textures::IRON] = textures::LoadRGBATexture("textures\\blocks\\iron.bmp", "");

	guiImage[1] = textures::LoadRGBATexture("textures\\gui\\MainMenu.bmp", "");
	guiImage[2] = textures::LoadRGBATexture("textures\\gui\\select.bmp", "");
	guiImage[3] = textures::LoadRGBATexture("textures\\gui\\unselect.bmp", "");
	guiImage[4] = textures::LoadRGBATexture("textures\\gui\\title.bmp", "textures\\gui\\titlemask.bmp");
	guiImage[5] = textures::LoadRGBATexture("textures\\gui\\lives.bmp", "");

	for (int gloop = 1; gloop != 11; gloop++){
		string path = "textures\\blocks\\destroy_" + itos(gloop) + ".bmp";
		DestroyImage[gloop] = textures::LoadRGBATexture(path, path);
	}

	BlockTextures = textures::LoadRGBATexture("textures\\blocks\\Terrain.bmp", "textures\\blocks\\Terrainmask.bmp");
}

void saveGame(){
	world::saveGame();
	player::save(world::worldname);
}

void loadGame(){
	world::loadGame();
	player::load(world::worldname);
}

void drawcloud(double px, double pz){

	//画云
	//Powered by bobby825(SuperSodaSea)
	static bool generated = false;
	static int d = 0;
	static float f = 0;
	int e;
	f += 0.125;
	e = int(f / cloudwidth);
	f -= e * cloudwidth;
	d += e;

	int cx, cz;
	px -= 32 * cloudwidth;
	pz -= 32 * cloudwidth;

	cx = int(px / cloudwidth);

	cz = int(pz / cloudwidth);

	int i, j;

	if (!generated){

		for (i = 0; i != 128; i++){
			for (j = 0; j != 128; j++){
				world::cloud[i][j] = sbyte(rnd() * 2);
			}
		}

		generated = true;
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);

	for (i = 0; i != 128; i++){
		for (j = 0; j != 128; j++){
			int x, z;
			int mx, mz;
			x = cx + i;
			z = cz + j;
			if (x < 0)
				mx = 127 + (x % 128);
			else
				mx = x % 128;

			if (z - d < 0)
				mz = 127 + ((z - d) % 128);
			else
				mz = (z - d) % 128;

			if (world::cloud[mx][mz]){
				float c = (128 - abs(i - 64) - abs(j - 64)) / 127.0f * 0.5f + 0.25f;

				glColor4f(1.0f, 1.0f, 1.0f, c);

				glVertex3f(x * (float)cloudwidth, 128.0f, z * (float)cloudwidth + f);
				glVertex3f(x * (float)cloudwidth, 128.0f, (z + 1) * (float)cloudwidth + f);
				glVertex3f((x + 1) * (float)cloudwidth, 128.0f, (z + 1) * (float)cloudwidth + f);
				glVertex3f((x + 1) * (float)cloudwidth, 128.0f, z * (float)cloudwidth + f);
			}
		}
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


void updategame(){

	Time_updategame_ = timer();

	double lx, ly, lz, sidedist[7];
	int sidedistmin;             //用于选择方块的变量;

	//按键是否按下
	static bool F1P;
	static bool F2P;
	static bool F3P;
	static bool F3_HP;
	static bool F3_CP;
	static bool F4P;
	static bool F5P;
	static bool LP;
	static bool SPACEP;
	static bool WP;
	static bool PP;
	static bool ZP;
	static bool XP;
	static bool enterp, enterpl;
	static bool tabp, tabpl;

	static double Wprstm;
	static bool puted;     //标准的chinglish吧。。。主要是put已经被FB作为关键字了。。。;

	BlockInHand = inventorybox[3][itemInHand];

	while (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS){
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwPollEvents();
		// glfwSleep(0.05)
	}

	//设置鼠标
	//glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN)
	glfwGetCursorPos(win, &mx, &my);
	if (!bagOpened)
		glfwSetCursorPos(win, windowwidth / 2, windowheight / 2);

	//获得鼠标按下情况
	mb = 0;
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mb = 1;
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mb = 2;

	//world::unloadedChunks=0
	world::rebuiltChunks = 0;
	world::updatedChunks = 0;

	//ciArray move
	if (world::ciArrayAval){
		if (world::ciArray.originX != player::cxt - viewdistance - 2 || world::ciArray.originY != player::cyt - viewdistance - 2 || world::ciArray.originZ != player::czt - viewdistance - 2){
			world::ciArray.moveTo(player::cxt - viewdistance - 2, player::cyt - viewdistance - 2, player::czt - viewdistance - 2);
		}
	}

	if (FirstUpdateThisFrame){

		//卸载区块(Unload chunks)
		world::sortChunkUnloadList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
		int sumUnload = world::chunkUnloads > 2 ? 2 : world::chunkUnloads;
		for (int i = 1; i <= sumUnload; i++){
			int cx = world::chunkUnloadList[i][1];
			int cy = world::chunkUnloadList[i][2];
			int cz = world::chunkUnloadList[i][3];
			world::getChunkPtr(cx, cy, cz)->Unload();
			world::DeleteChunk(cx, cy, cz);
		}

		//加载区块(Load chunks)
		world::sortChunkLoadList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
		int sumLoad = world::chunkLoads > 2 ? 3 : world::chunkLoads;
		for (int i = 1; i < sumLoad; i++){
			int cx = world::chunkLoadList[i][1];
			int cy = world::chunkLoadList[i][2];
			int cz = world::chunkLoadList[i][3];

			world::AddChunk(cx, cy, cz);
			world::getChunkPtr(cx, cy, cz)->Load();
		}
		if (rnd() < 0.1) world::MOs.push_back(shared_ptr<Mo>(new Cat(player::xa + rand() % 40 - 20, player::ya, player::za + rand() % 40 - 20)));
	}

	//加载动画
	for (int i = 0; i != world::loadedChunks; i++){
		world::chunk* cp = &world::chunks[i];
		if (cp->loadAnim <= 0.3f)
			cp->loadAnim = 0.0f;
		else
			cp->loadAnim *= 0.6f;
	}
	//随机状态更新
	for (int i = 0; i != world::loadedChunks; i++){
		int x, y, z;
		int cx = world::chunks[i].cx;
		int cy = world::chunks[i].cy;
		int cz = world::chunks[i].cz;
		x = int(rnd() * 16);
		y = int(rnd() * 16);
		z = int(rnd() * 16);
		if (world::chunks[i].getblock(x, y, z) == blocks::DIRT &&
			world::getblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16) == blocks::AIR &&
			(world::getblock(x + cx * 16 + 1, y + cy * 16, z + cz * 16) == blocks::GRASS ||
			world::getblock(x + cx * 16 - 1, y + cy * 16, z + cz * 16) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16, z + cz * 16 + 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 - 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 - 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 - 1, z + cz * 16 - 1) == blocks::GRASS ||
			world::getblock(x + cx * 16, y + cy * 16 - 1, z + cz * 16 - 1) == blocks::GRASS)){
			world::chunks[i].setblock(x, y, z, blocks::GRASS);
			world::updateblock(x + cx * 16, y + cy * 16 + 1, z + cz * 16, true);
			world::setChunkUpdated(cx, cy, cz, true);
		}
	}
	//判断选中的方块
	lx = player::xpos; ly = player::ypos + playerHeight + playerHeightExt; lz = player::zpos;

	selx = 0;
	sely = 0;
	selz = 0;
	selbx = 0;
	selby = 0;
	selbz = 0;
	selcx = 0;
	selcy = 0;
	selcz = 0;
	selb = 0;
	selbr = 0;
	puted = false;
	if (!bagOpened){
		//从玩家位置发射一条线段
		for (int i = 0; i < selectPrecision*selectDistance; i++){
			//线段延伸
			lx += sin(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			ly += cos(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			lz += cos(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;

			//碰到方块
			if (BlockInfo(world::getblock(RoundInt(lx), RoundInt(ly), RoundInt(lz))).isSolid()){
				int x, y, z;
				x = RoundInt(lx);
				y = RoundInt(ly);
				z = RoundInt(lz);

				selx = x; sely = y; selz = z;

				//找方块所在区块及位置
				selcx = world::getchunkpos(x);
				selbx = world::getblockpos(x);
				selcy = world::getchunkpos(y);
				selby = world::getblockpos(y);
				selcz = world::getchunkpos(z);
				selbz = world::getblockpos(z);

				sidedist[1] = abs(y + 0.5 - ly);          //顶面;
				sidedist[2] = abs(y - 0.5 - ly);		  //底面;
				sidedist[3] = abs(x + 0.5 - lx);		  //左面;
				sidedist[4] = abs(x - 0.5 - lx);	   	  //右面;
				sidedist[5] = abs(z + 0.5 - lz);		  //前面;
				sidedist[6] = abs(z - 0.5 - lz);		  //后面;
				sidedistmin = 1;						  //离哪个面最近;
				for (int j = 2; j <= 6; j++){
					if (sidedist[j] < sidedist[sidedistmin]) sidedistmin = j;
				}

				if (world::chunkOutOfBound(selcx, selcy, selcz) == false){
					selb = world::getChunkPtr(selcx, selcy, selcz)->getblock(selbx, selby, selbz);
				}

				switch (sidedistmin){
				case 1:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx, sely + 1, selz);
					break;
				case 2:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx, sely - 1, selz);
					break;
				case 3:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx + 1, sely, selz);
					break;
				case 4:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx - 1, sely, selz);
					break;
				case 5:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx, sely, selz + 1);
					break;
				case 6:
					if (world::chunkOutOfBound(selcx, selcy, selcz) == false)
						selbr = world::getbrightness(selx, sely, selz - 1);
					break;
				}

				if (mb == 1 || enterp){
					particles::throwParticle(world::getblock(x, y, z),
						float(x + rnd() - 0.5f), float(y + rnd() - 0.2f), float(z + rnd() - 0.5f),
						float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f),
						float(rnd()*0.01f + 0.02f), int(rnd() * 30) + 30);

					if (selx != oldselx || sely != oldsely || selz != oldselz)
						seldes = 0.0;
					else
						seldes += 5.0;
					if (seldes >= 100.0){
						additem(world::getblock(x, y, z));
						for (int j = 1; j <= 25; j++){
							particles::throwParticle(world::getblock(x, y, z),
								float(x + rnd() - 0.5f), float(y + rnd() - 0.2f), float(z + rnd() - 0.5f),
								float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f),
								float(rnd()*0.02 + 0.03), int(rnd() * 60) + 30);
						}
						world::pickblock(x, y, z);
					}
				}
				//放置方块
				if ((mb == 2 && mbp == false) || (tabp && tabpl == false) && inventorypcs[3][itemInHand] > 0){
					puted = true;
					switch (sidedistmin){
					case 1:
						if (player::putblock(x, y + 1, z, BlockInHand) == false) puted = false;
						break;
					case 2:
						if (player::putblock(x, y - 1, z, BlockInHand) == false) puted = false;
						break;
					case 3:
						if (player::putblock(x + 1, y, z, BlockInHand) == false) puted = false;
						break;
					case 4:
						if (player::putblock(x - 1, y, z, BlockInHand) == false) puted = false;
						break;
					case 5:
						if (player::putblock(x, y, z + 1, BlockInHand) == false) puted = false;
						break;
					case 6:
						if (player::putblock(x, y, z - 1, BlockInHand) == false) puted = false;
						break;
					}
					if (puted){
						inventorypcs[3][itemInHand]--;
						if (inventorypcs[3][itemInHand] == 0) inventorypcs[3][itemInHand] = blocks::AIR;
					}
				}
				break;
			}
		}

		if (selx != oldselx || sely != oldsely || selz != oldselz || (mb == 0 && enterp == false)) seldes = 0.0;
		oldselx = selx;
		oldsely = sely;
		oldselz = selz;

		player::intxpos = RoundInt(player::xpos);
		player::intypos = RoundInt(player::ypos);
		player::intzpos = RoundInt(player::zpos);

		//if (player::lives > player::livestarget) player::lives--
		//if (player::lives < player::livestarget) player::lives++

		//移动！(生命在于运动)

		if (glfwGetKey(win, GLFW_KEY_W) == 1){
			if (!WP){
				if (Wprstm == 0.0){
					Wprstm = timer();
				}
				else{
					if (timer() - Wprstm <= 0.5){ player::Running = true; Wprstm = 0.0; }
					else Wprstm = timer();
				}
			}
			if (Wprstm != 0.0 && timer() - Wprstm > 0.5) Wprstm = 0.0;
			WP = true;
			player::xa = -sin(player::heading*M_PI / (double)180) * player::speed;
			player::za = -cos(player::heading*M_PI / (double)180) * player::speed;
		}
		else{
			player::Running = false;
			WP = false;
		}
		if (player::Running){
			player::speed = runspeed;
			if (FOVyExt<9.8)
				FOVyExt = 10.0f - (10.0f - FOVyExt)*0.8f;
			else
				FOVyExt = 10.0f;
		}
		else{
			player::speed = walkspeed;
			if (FOVyExt>0.2)
				FOVyExt *= 0.8f;
			else
				FOVyExt = 0.0f;
		}

		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS){
			player::xa = sin(player::heading*M_PI / (double)180) * player::speed;
			player::za = cos(player::heading*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS){
			player::xa = sin((player::heading - 90)*M_PI / (double)180) * player::speed;
			player::za = cos((player::heading - 90)*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS){
			player::xa = -sin((player::heading - 90)*M_PI / (double)180) * player::speed;
			player::za = -cos((player::heading - 90)*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS){
			player::xa = -sin(player::heading*M_PI / (double)180) * runspeed * 10;
			player::za = -cos(player::heading*M_PI / (double)180) * runspeed * 10;
		}

		if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS){
			player::xa = sin(player::heading*M_PI / (double)180) * runspeed * 10;
			player::za = cos(player::heading*M_PI / (double)180) * runspeed * 10;
		}

		//切换方块

		if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS){
			if (itemInHand > 0 && ZP == false) itemInHand--;
			ZP = true;
		}
		else{
			ZP = false;
		}
		if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS){
			if (itemInHand < 9 && XP == false) itemInHand++;
			XP = true;
		}
		else
		{
			XP = false;
		}

		//if (itemInHand + (mwl - mw) <= 9 && mwl>mw) itemInHand += mwl - mw
		//if (itemInHand - (mw - mwl) >= 0 && mw > mwl) itemInHand -= mw - mwl
		/*BlockInHand = inventorybox[3][itemInHand];*/
		mwl = mw;

		//转头！你治好了我多年的颈椎病！

		if (mx > int(windowwidth / 2))
			player::heading -= (mx - int(windowwidth / 2))*mousemove;

		if (mx < int(windowwidth / 2))
			player::heading += (int(windowwidth / 2) - mx)*mousemove;

		if (my<int(windowheight / 2))
			player::lookupdown -= (int(windowheight / 2) - my)*mousemove;

		if (my>windowheight / 2)
			player::lookupdown += (my - int(windowheight / 2))*mousemove;


		if (glfwGetKey(win, GLFW_KEY_RIGHT) == 1){
			player::xlookspeed -= mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT) == 1){
			player::xlookspeed += mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_UP) == 1){
			player::ylookspeed -= mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_DOWN) == 1){
			player::ylookspeed += mousemove * 4;
		}

		player::heading += player::xlookspeed;
		player::lookupdown += player::ylookspeed;
		player::xlookspeed *= 0.8f;
		player::ylookspeed *= 0.8f;
	}

	//限制角度，别把头转掉下来了 ←_←
	if (player::lookupdown <-90) player::lookupdown = -90;
	if (player::lookupdown > 90) player::lookupdown = 90;

	//if (player::lookupdown <-120) player::lookupdown = -120
	//if (player::lookupdown > 120) player::lookupdown = 120
	if (!bagOpened){

		enterpl = enterp;
		if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS){
			enterp = true;
		}
		if (glfwGetKey(win, GLFW_KEY_ENTER) != GLFW_PRESS) enterp = false;

		tabpl = tabp;
		if (glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS){
			tabp = true;
		}
		if (glfwGetKey(win, GLFW_KEY_TAB) != GLFW_PRESS) tabp = false;

		//起跳！
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS && SPACEP == false){
			if (!player::inWater) {
				SPACEP = true;
				if ((player::OnGround || player::AirJumps < MaxAirJumps) && FLY == false && CROSS == false){
					if (player::OnGround == false) player::AirJumps++;
					player::jump = 0.4; player::OnGround = false;
				}
				if (FLY || CROSS) { player::ya += walkspeed; SPACEP = false; }
				Wprstm = 0.0;
			}
			else{
				player::ya = walkspeed;
			}
		}
		if (glfwGetKey(win, GLFW_KEY_SPACE) != GLFW_PRESS) SPACEP = false;

		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
			if (CROSS || FLY){
				player::ya -= walkspeed;
			}
			else{
				if (playerHeightExt > -0.59f)  playerHeightExt -= 0.1f; else playerHeightExt = -0.6f;
			}
			Wprstm = 0.0;
		}

		//各种设置切换

		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS && F1P == false){
			FLY = !FLY;
			F1P = true;
			player::jump = 0.0;
		}
		if (glfwGetKey(win, GLFW_KEY_F1) != GLFW_PRESS) F1P = false;

		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS && F2P == false){
			shouldGetScreenshot = true;
			F2P = true;
		}
		if (!glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS) F2P = false;

		if (glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS && F3P == false){
			DEBUGMODE = !DEBUGMODE;
			F3P = true;
		}
		if (!glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS) F3P = false;

		if (glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && F3_HP == false){
			DebugHitbox = !DebugHitbox;
			DEBUGMODE = true;
			F3_HP = true;
		}
		if (!(glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS)) F3_HP = false;

		if (glfwGetKey(win, GLFW_KEY_F4) == GLFW_PRESS && F4P == false){
			CROSS = !CROSS;
			F4P = true;
		}
		if (glfwGetKey(win, GLFW_KEY_F4) != GLFW_PRESS) F4P = false;
		if (glfwGetKey(win, GLFW_KEY_F5) == GLFW_PRESS && F5P == false){
			GUIrenderswitch = !GUIrenderswitch;
			F5P = true;

		}
		if (glfwGetKey(win, GLFW_KEY_F5) != GLFW_PRESS) F5P = false;
	}

	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS && EP == false){
		bagOpened = !bagOpened;
		if (!bagOpened){
			glfwSetCursorPos(win, windowwidth / 2, windowheight / 2);
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else{
			shouldGetThumbnail = true;
		}
		EP = true;
	}
	if (glfwGetKey(win, GLFW_KEY_E) != GLFW_PRESS) EP = false;

	if (!bagOpened){
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS) ESCP = false;

		if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS && LP == false){
			world::saveAllChunks();
			LP = true;
		}
		if (glfwGetKey(win, GLFW_KEY_L) != GLFW_PRESS) LP = false;
	}
	//跳跃
	if (!player::inWater){
		if (FLY == false && CROSS == false){

			player::ya = -0.001;

			if (player::OnGround){
				player::jump = 0.0;
				player::AirJumps = 0;
				SPACEP = false;
			}
			else{
				player::jump -= 0.02;
				player::ya = player::jump;
			}
		}
		else{

			player::jump = 0.0;
			player::AirJumps = 0;

		}
	}
	else{
		player::jump = 0.0;
		player::AirJumps = 0;
		SPACEP = false;
		if (player::ya <= 0.001){
			player::ya = -0.001;
			if (!player::OnGround) player::ya = -0.1;
		}

	}

	if (player::NearWall && FLY == false && CROSS == false){
		//player::ya += walkspeed
		//player::jump = 0.0

		//爬墙？
	}

	mbp = mb;

	player::intxpos = RoundInt(player::xpos);
	player::intypos = RoundInt(player::ypos);
	player::intzpos = RoundInt(player::zpos);
	player::Move();
	player::xposold = player::xpos;
	player::yposold = player::ypos;
	player::zposold = player::zpos;
	player::intxposold = RoundInt(player::xpos);
	player::intyposold = RoundInt(player::ypos);
	player::intzposold = RoundInt(player::zpos);
	particles::updateall();

	Time_updategame += timer() - Time_updategame;

}

void show(string s, bool init = false){
	static int pos = 0;
	if (init){
		pos = 0;
		return;
	}
	glprint(0, 16 * pos, s);
	pos++;
}

void drawGUI(){

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	int seldes_100 = int(seldes / 100);
	if (DEBUGMODE){

		if (selb != blocks::AIR){

			glLineWidth(1);
			glBegin(GL_LINES);
			glColor4f(gui::FgR, gui::FgG, gui::FgB, 0.8f);
			glVertex2i(windowwidth / 2, windowheight / 2);
			glVertex2i(windowwidth / 2 + 50, windowheight / 2 + 50);
			glVertex2i(windowwidth / 2 + 50, windowheight / 2 + 50);
			glVertex2i(windowwidth / 2 + 250, windowheight / 2 + 50);
			glEnd();
			setFontColor(1.0f, 1.0f, 1.0f, 0.8f);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_CULL_FACE);
			std::stringstream ss;
			ss << BlockInfo(selb).getBlockName() << " (ID " << (int)selb << ")";
			glprint(windowwidth / 2 + 50, windowheight / 2 + 50 - 16, ss.str());
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);
			glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
		}
		else{
			glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
		}

		glLineWidth(2);

		glBegin(GL_LINES);
		
		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 - linedist + linelength + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + linelength + seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);

		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 - linedist + linelength + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - linelength - seldes_100 * linedist, windowheight / 2 - linedist + seldes_100 * linedist);

		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 + linedist - linelength - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 - linedist + linelength + seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);

		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 + linedist - linelength - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);
		glVertex2i(windowwidth / 2 + linedist - linelength - seldes_100 * linedist, windowheight / 2 + linedist - seldes_100 * linedist);

		glEnd();

	}

	glLineWidth(4);
	glBegin(GL_LINES);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glVertex2i(windowwidth / 2 - 20, windowheight / 2);
	glVertex2i(windowwidth / 2 + 20, windowheight / 2);
	glVertex2i(windowwidth / 2, windowheight / 2 - 20);
	glVertex2i(windowwidth / 2, windowheight / 2 + 20);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glVertex2i(windowwidth / 2 - 15, windowheight / 2);
	glVertex2i(windowwidth / 2 + 15, windowheight / 2);
	glVertex2i(windowwidth / 2, windowheight / 2 - 15);
	glVertex2i(windowwidth / 2, windowheight / 2 + 15);
	glEnd();

	if (seldes>0.0){

		glBegin(GL_LINES);

		glColor4f(0.5, 0.5, 0.5, 1.0);

		glVertex2i(windowwidth / 2 - 15, windowheight / 2);
		glVertex2i(windowwidth / 2 - 15 + seldes_100 * 15, windowheight / 2);

		glVertex2i(windowwidth / 2 + 15, windowheight / 2);
		glVertex2i(windowwidth / 2 + 15 - seldes_100 * 15, windowheight / 2);

		glVertex2i(windowwidth / 2, windowheight / 2 - 15);
		glVertex2i(windowwidth / 2, windowheight / 2 - 15 + seldes_100 * 15);

		glVertex2i(windowwidth / 2, windowheight / 2 + 15);
		glVertex2i(windowwidth / 2, windowheight / 2 + 15 - seldes_100 * 15);

		glEnd();

	}

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i != 10; i++){
		if (i == itemInHand)
			glBindTexture(GL_TEXTURE_2D, guiImage[2]);
		else
			glBindTexture(GL_TEXTURE_2D, guiImage[3]);

		glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
		glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex2d((i)*(32), windowheight - 32);
		glTexCoord2f(0.0, 0.0);
		glVertex2d((i)*(32) + 32, windowheight - 32);
		glTexCoord2f(1.0, 0.0);
		glVertex2d((i)*(32) + 32, windowheight);
		glTexCoord2f(1.0, 1.0);
		glVertex2d((i)*(32), windowheight);
		glEnd();
		if (inventorybox[3][i] != blocks::AIR){
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex(inventorybox[3][i], 1)]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex2d(i * 32 + 2, windowheight - 32 + 2);
			glTexCoord2f(0.0, 0.0);
			glVertex2d(i * 32 + 30, windowheight - 32 + 2);
			glTexCoord2f(1.0, 0.0);
			glVertex2d(i * 32 + 30, windowheight - 32 + 30);
			glTexCoord2f(1.0, 1.0);
			glVertex2d(i * 32 + 2, windowheight - 32 + 30);
			glEnd();
			std::stringstream ss;
			ss << (int)inventorypcs[3][i];
			glprint(i * 32, windowheight - 32, ss.str());
		}
	}

	//glDisable(GL_TEXTURE_2D)
	//glColor4f(1.0 - player::lives / (double)player::livesoverall, player::lives / (double)player::livesoverall, 0.0, 0.5)

	//glBegin(GL_QUADS)
	//glVertex2d(windowwidth - 256, windowheight)
	//glVertex2d(windowwidth - 256, windowheight - 20)
	//glVertex2d(windowwidth - 256 + (player::lives / (double)player::livesoverall * 256), windowheight - 20)
	//glVertex2d(windowwidth - 256 + (player::lives / (double)player::livesoverall * 256), windowheight)
	//glEnd()

	//glDisable(GL_TEXTURE_2D)
	//glColor4f(0.0, 0.0, 0.0, 0.9)
	//glLineWidth(2)

	//glBegin(GL_LINES)
	//glVertex2d(windowwidth - 256 + (player::livestarget / (double)player::livesoverall * 256), windowheight - 20)
	//glVertex2d(windowwidth - 256 + (player::livestarget / (double)player::livesoverall * 256), windowheight)
	//glEnd()

	//glEnable(GL_TEXTURE_2D)

	if (DEBUGMODE){
		setFontColor(1.0f, 1.0f, 1.0f, 0.9f);
		std::stringstream ss;
		ss << "NEWorld v: " << VERSION << " [OpenGL " << GLVersionMajor << "." << GLVersionMinor << "|" << GLVersionRev << "]";
		show(ss.str()); ss.str("");
		ss << "Flying: " << boolstr(FLY);
		show(ss.str()); ss.str("");
		ss << "Render GUI: " << boolstr(GUIrenderswitch);
		show(ss.str()); ss.str("");
		ss << "Debug Mode: " << boolstr(DEBUGMODE);
		show(ss.str()); ss.str("");
		ss << "Crosswall: " << boolstr(CROSS);
		show(ss.str()); ss.str("");
		ss << "Block: " << BlockInfo(BlockInHand).getBlockName() << " (ID" << (int)BlockInHand << ")";
		show(ss.str()); ss.str("");
		ss << "Fps: " << fps;
		show(ss.str()); ss.str("");
		ss << "Ups(Tps): " << ups;
		show(ss.str()); ss.str("");

		ss << "Xpos: " << player::xpos;
		show(ss.str()); ss.str("");
		ss << "Ypos: " << player::ypos;
		show(ss.str()); ss.str("");
		ss << "Zpos: " << player::zpos;
		show(ss.str()); ss.str("");
		ss << "Direction: " << player::heading;
		show(ss.str()); ss.str("");
		ss << "Head: " << player::lookupdown;
		show(ss.str()); ss.str("");
		ss << "On ground: " << boolstr(player::OnGround);
		show(ss.str()); ss.str("");
		ss << "Jump speed: " << player::jump;
		show(ss.str()); ss.str("");
		ss << "Near wall: " << boolstr(player::NearWall);
		show(ss.str()); ss.str("");
		ss << "In water: " << boolstr(player::inWater);
		show(ss.str()); ss.str("");

		ss << world::loadedChunks << " chunks loaded";
		show(ss.str()); ss.str("");
		ss << renderedChunk << " chunks rendered";
		show(ss.str()); ss.str("");
		ss << world::unloadedChunks << " chunks unloaded";
		show(ss.str()); ss.str("");
		ss << world::updatedChunks << " chunks updated";
		show(ss.str()); ss.str("");

		ss << "ChunkIndexCache Index = " << world::ciCacheIndex;
		show(ss.str()); ss.str("");

		ss << "MO count: " << world::MOs.size();
		show(ss.str()); ss.str("");

		//ss << "Particle count: " & particles::ptcsrendered & "/" & particles::ptcs.size()
		//show(ss.str())

		show("", true);
		//setFontColor(gui::FgR, gui::FgG, gui::FgB, 0.9)
		//
		//glprint(windowwidth - 300, 16 * 0, ("Block infomation"))
		//
		//glprint(windowwidth - 300, 16 * 1, ("bx" << str(selbx) << " by" << str(selby) << " bz" << str(selbz)))
		//
		//glprint(windowwidth - 300, 16 * 2, ("cx" << str(selcx) << " cy" << str(selcy) << " cz" << str(selcz)))
		//
		//glprint(windowwidth - 300, 16 * 3, ("Selected block" << blocks::IDstr(selb) << " (ID" << selb << ")"))
		//
		//glprint(windowwidth - 300, 16 * 4, ("Selected face brightness" << selbr))


		setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor4f(gui::FgR, gui::FgG, gui::FgB, 0.7f);

		glVertex2d(windowwidth - 300, 16 * 9 - 1);
		glVertex2d(windowwidth - 300, 16 * 10 - 1);
		glVertex2d(windowwidth - 300 + Time_renderscene / (double)Time_overall * 300, 16 * 10 - 1);
		glVertex2d(windowwidth - 300 + Time_renderscene / (double)Time_overall * 300, 16 * 9 - 1);

		glVertex2d(windowwidth - 300, 16 * 10 - 1);
		glVertex2d(windowwidth - 300, 16 * 11 - 1);
		glVertex2d(windowwidth - 300 + Time_renderGUI / (double)Time_overall * 300, 16 * 11 - 1);
		glVertex2d(windowwidth - 300 + Time_renderGUI / (double)Time_overall * 300, 16 * 10 - 1);

		glVertex2d(windowwidth - 300, 16 * 11 - 1);
		glVertex2d(windowwidth - 300, 16 * 12 - 1);
		glVertex2d(windowwidth - 300 + Time_updategame / (double)Time_overall * 300, 16 * 12 - 1);
		glVertex2d(windowwidth - 300 + Time_updategame / (double)Time_overall * 300, 16 * 11 - 1);

		glVertex2d(windowwidth - 300, 16 * 12 - 1);
		glVertex2d(windowwidth - 300, 16 * 13 - 1);
		glVertex2d(windowwidth - 300 + Time_screensync / (double)Time_overall * 300, 16 * 13 - 1);
		glVertex2d(windowwidth - 300 + Time_screensync / (double)Time_overall * 300, 16 * 12 - 1);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
		glVertex2d(windowwidth, 16 * 9 - 1);
		glVertex2d(windowwidth, 16 * 10 - 1);
		glVertex2d(windowwidth - 300, 16 * 10 - 1);
		glVertex2d(windowwidth - 300, 16 * 9 - 1);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(windowwidth, 16 * 10 - 1);
		glVertex2d(windowwidth, 16 * 11 - 1);
		glVertex2d(windowwidth - 300, 16 * 11 - 1);
		glVertex2d(windowwidth - 300, 16 * 10 - 1);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(windowwidth, 16 * 11 - 1);
		glVertex2d(windowwidth, 16 * 12 - 1);
		glVertex2d(windowwidth - 300, 16 * 12 - 1);
		glVertex2d(windowwidth - 300, 16 * 11 - 1);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(windowwidth, 16 * 12 - 1);
		glVertex2d(windowwidth, 16 * 13 - 1);
		glVertex2d(windowwidth - 300, 16 * 13 - 1);
		glVertex2d(windowwidth - 300, 16 * 12 - 1);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		setFontColor(0.0f, 0.0f, 0.0f, 0.3f);
		glprint(windowwidth - 300, 16 * 9, "Render scene");
		glprint(windowwidth - 300, 16 * 10, "Render GUI");
		glprint(windowwidth - 300, 16 * 11, "Update game");
		glprint(windowwidth - 300, 16 * 12, "Others");
		setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else{

		setFontColor(1.0f, 1.0f, 1.0f, 0.9f);
		std::stringstream ss;
		ss << "v" << VERSION;
		glprint(0, 0, ss.str());

	}

	//检测帧速率
	if (timer() - fctime >= 1){
		fps = fpsc;
		fpsc = 0;
		fctime = timer();
	}
	fpsc++;

	//glDisable GL_CULL_FACE
	//glColor4f 0.0,0.0,0.0,1.0
	//selectFont(48,ANSI_CHARSET,"Courier New")
	//glRterPos2i(100,100)
	//DrawString("NEWorld!!!!!!")

	Time_renderGUI = timer() - Time_renderGUI;
	Time_screensync_ = timer();
}

void drawmain(){
	//画场景

	Time_renderscene_ = timer();

	int cx = 0, cy = 0, cz = 0;
	double lx, ly, lz;
	//double ticktime, ftime

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVyNormal + FOVyExt, windowwidth / (double)windowheight, 0.1, 512.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(player::lookupdown, 1, 0, 0);
	glRotated(360.0 - player::heading, 0, 1, 0);
	glTranslated(-player::xpos, -player::ypos - playerHeight - playerHeightExt, -player::zpos);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Frustum::calc();

	glColor4f(1.0, 1.0, 1.0, 1.0);

	player::cxt = world::getchunkpos((int)player::xpos);
	player::cyt = world::getchunkpos((int)player::ypos);
	player::czt = world::getchunkpos((int)player::zpos);

	//更新区块显示列表
	world::sortChunkRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
	for (int i = 1; i <= world::chunkRenders; i++){
		cx = world::chunkRenderList[i][1];
		cy = world::chunkRenderList[i][2];
		cz = world::chunkRenderList[i][3];
		auto cptr = world::getChunkPtr(cx, cy, cz);
		if (cptr->getUpdated()){
			cptr->buildlists();
		}
	}
	//uint lists_(0 to - 1)
	//double loadAnims_(0 to - 1)
	vector<uint> lists_;
	vector<double> loadAnims_;
	renderedChunk = 0;
	for (int i = 0; i != world::loadedChunks; i++){
		if (world::chunkInRange(world::chunks[i].cx, world::chunks[i].cy, world::chunks[i].cz, player::cxt, player::cyt, player::czt, viewdistance)){
			if (world::chunks[i].list > 0){
				auto chunknow = world::chunks[i];
				if (Frustum::aabbInFrustum(chunknow.getChunkAABB())){
					lists_.push_back(chunknow.list);
					loadAnims_.push_back(chunknow.loadAnim);
					renderedChunk++;
				}
			}
		}
	}
	int lists_count = lists_.size();
	MutexUnlock(Mutex);
	for (int i = 0; i != lists_count; i++){
		glTranslatef(0.0f, (float)-loadAnims_[i], 0.0f);
		if (lists_[i] > 0 && glIsList(lists_[i])) glCallList(lists_[i]);
		glTranslatef(0.0f, (float)loadAnims_[i], 0.0f);
	}
	MutexLock(Mutex);

	if (seldes > 0.0){
		renderDestroy(seldes, selx, sely, selz);
	}
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	particles::renderall();

	glDisable(GL_TEXTURE_2D);

	if (GUIrenderswitch){
		lx = player::xpos; ly = player::ypos + playerHeight + playerHeightExt; lz = player::zpos;
		for (int i = 1; i <= selectPrecision*selectDistance; i++){
			lx += sin(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / selectPrecision;
			ly += cos(M_PI / 180 * (player::lookupdown + 90)) / selectPrecision;
			lz += cos(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / selectPrecision;
			if (BlockInfo(world::getblock(RoundInt(lx), RoundInt(ly), RoundInt(lz))).isSolid()){
				drawborder(RoundInt(lx), RoundInt(ly), RoundInt(lz));
				break;
			}
		}
	}

	MutexUnlock(Mutex);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	for (int i = 0; i != lists_count; i++){
		glTranslatef(0.0f, (float)-loadAnims_[i], 0.0f);
		if (lists_[i] > 1 && glIsList(lists_[i] + 1)) glCallList(lists_[i] + 1);
		glTranslatef(0.0f, (float)loadAnims_[i], 0.0f);
	}
	glDisable(GL_CULL_FACE);
	for (int i = 0; i != lists_count; i++){
		glTranslatef(0.0f, (float)-loadAnims_[i], 0.0f);
		if (lists_[i] > 2 && glIsList(lists_[i] + 2)) glCallList(lists_[i] + 2);
		glTranslatef(0.0f, (float)loadAnims_[i], 0.0f);
	}

	glDisable(GL_TEXTURE_2D);
	drawcloud(player::xpos, player::zpos);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	MutexLock(Mutex);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glDepthFunc(GL_ALWAYS);

	if (world::getblock((int)player::xpos, int(player::ypos + playerHeight + playerHeightExt), (int)player::zpos) == blocks::WATER){
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex(blocks::WATER, 1)]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2i(0, 0);
		glTexCoord2f(0.0, 0.0); glVertex2i(0, windowheight);
		glTexCoord2f(1.0, 0.0); glVertex2i(windowwidth, windowheight);
		glTexCoord2f(1.0, 1.0); glVertex2i(windowwidth, 0);
		glEnd();
	}

	Time_renderscene = timer() - Time_renderscene;
	Time_renderGUI_ = timer();

	if (GUIrenderswitch){
		drawGUI();
	}
	if (bagOpened)
		bagUpdate();
	else
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (shouldGetScreenshot){
		shouldGetScreenshot = false;
		time_t t = time(0);
		char tmp[64];
		tm timeinfo;
		localtime_s(&timeinfo, &t);
		strftime(tmp, sizeof(tmp), "%Y年%m月%d日%H时%M分%S秒", &timeinfo);
		std::stringstream ss;
		ss << "\\screenshots\\" << tmp << ".bmp";
		saveScreenshot(0, 0, windowwidth, windowheight, ss.str());
	}
	if (shouldGetThumbnail){
		shouldGetThumbnail = false;
		createThumbnail();
	}

	//if( versync ) screensync=true

	//屏幕刷新，千万别删，后果自负！！！
	//====refresh====//
	MutexUnlock(Mutex);
	glfwSwapBuffers(win);
	glfwPollEvents();
	MutexLock(Mutex);
	//==refresh end==//

	Time_screensync = timer() - Time_screensync;

}

//绘制选择边框，建议用GL_LINE_LOOP，别学我QAQ
void drawborder(int x, int y, int z){
	static float extrize = 0.002f; //实际上这个边框应该比方块大一些，否则很难看;
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	// Top Face
	glBegin(GL_LINES);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glEnd();
	// Bottom Face
	glBegin(GL_LINES);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glEnd();
	// Left Face
	glBegin(GL_LINES);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glEnd();
	// Right Face
	glBegin(GL_LINES);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glEnd();
	// Front Face
	glBegin(GL_LINES);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, (0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, (0.5f + extrize) + z);
	glEnd();
	// Back Face
	glBegin(GL_LINES);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, (0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f(-(0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glVertex3f((0.5f + extrize) + x, -(0.5f + extrize) + y, -(0.5f + extrize) + z);
	glEnd();
	glLineWidth(1);
	glDisable(GL_LINE_SMOOTH);
}

void renderDestroy(float level, int x, int y, int z){

	//float colors
	static float ES = 0.002f;

	glColor4f(1.0, 1.0, 1.0, 1.0);

	if (level < 100.0){
		glBindTexture(GL_TEXTURE_2D, DestroyImage[int(level / 10) + 1]);
	}
	else{
		glBindTexture(GL_TEXTURE_2D, DestroyImage[10]);
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f((0.5f + ES) + x, (0.5f + ES) + y, -(0.5f + ES) + z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, -(0.5f + ES) + z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f((0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-(0.5f + ES) + x, -(0.5f + ES) + y, (0.5f + ES) + z);
	glEnd();
}

void bagUpdate(){
	//背包界面
	static int si, sj, sf;
	int leftp = (windowwidth - 392) / 2;
	int upp = windowheight - 152 - 16;
	static int mousew, mouseb, mousebl;
	double mousex, mousey;
	static block itemselected = blocks::AIR;
	static block pcsselected = 0;
	glClearColor(skycolorR, skycolorG, skycolorB, 1.0);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwGetCursorPos(win, &mousex, &mousey);
	mousew = mw; //?
	mouseb = mb;
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(0.2f, 0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(windowwidth, 0);
	glVertex2i(windowwidth, windowheight);
	glVertex2i(0, windowheight);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	sf = 0;
	for (int i = 0; i != 4; i++){
		for (int j = 0; j != 10; j++){
			if (mousex >= j*(32 + 8) + leftp && mousex <= j*(32 + 8) + 32 + leftp &&
				mousey >= i*(32 + 8) + upp && mousey <= i*(32 + 8) + 32 + upp){
				si = i; sj = j; sf = 1;
				glBindTexture(GL_TEXTURE_2D, guiImage[2]);
				if (mousebl == 0 && mouseb == 1 && itemselected == inventorybox[i][j]){
					if (inventorypcs[i][j] + pcsselected <= 255){
						inventorypcs[i][j] += pcsselected;
						pcsselected = 0;
					}
					else
					{
						pcsselected = inventorypcs[i][j] + pcsselected - 255;
						inventorypcs[i][j] = 255;
					}
				}
				if (mousebl == 0 && mouseb == 1 && itemselected != inventorybox[i][j]){
					std::swap(pcsselected, inventorypcs[i][j]);
					std::swap(itemselected, inventorybox[i][j]);
				}
				if (mousebl == 0 && mouseb == 2 && itemselected == inventorybox[i][j] && inventorypcs[i][j] < 255){
					pcsselected--;
					inventorypcs[i][j]++;
				}
				if (mousebl == 0 && mouseb == 2 && inventorybox[i][j] == blocks::AIR){
					pcsselected--;
					inventorypcs[i][j] = 1;
					inventorybox[i][j] = itemselected;
				}

				if (pcsselected == 0) itemselected = blocks::AIR;
				if (itemselected == blocks::AIR) pcsselected = 0;
				if (inventorypcs[i][j] == 0) inventorybox[i][j] = blocks::AIR;
				if (inventorybox[i][j] == blocks::AIR) inventorypcs[i][j] = 0;
			}
			else{
				glBindTexture(GL_TEXTURE_2D, guiImage[3]);
			}
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex2d(j*(32 + 8) + leftp, i*(32 + 8) + upp);
			glTexCoord2f(0.0, 0.0);
			glVertex2d(j*(32 + 8) + 32 + leftp, i*(32 + 8) + upp);
			glTexCoord2f(1.0, 0.0);
			glVertex2d(j*(32 + 8) + 32 + leftp, i*(32 + 8) + 32 + upp);
			glTexCoord2f(1.0, 1.0);
			glVertex2d(j*(32 + 8) + leftp, i*(32 + 8) + 32 + upp);
			glEnd();
			if (inventorybox[i][j] != blocks::AIR){
				glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex((inventorybox[i][j]), 1)]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2d(j*(32 + 8) + 2 + leftp, i*(32 + 8) + 2 + upp);
				glTexCoord2f(1.0, 0.0);
				glVertex2d(j*(32 + 8) + 30 + leftp, i*(32 + 8) + 2 + upp);
				glTexCoord2f(1.0, 1.0);
				glVertex2d(j*(32 + 8) + 30 + leftp, i*(32 + 8) + 30 + upp);
				glTexCoord2f(0.0, 1.0);
				glVertex2d(j*(32 + 8) + 2 + leftp, i*(32 + 8) + 30 + upp);
				glEnd();
				std::stringstream ss;
				ss << (int)inventorypcs[i][j];
				glprint(j*(32 + 8) + 8 + leftp, (i*(32 + 16) + 8 + upp), ss.str());
			}
		}
	}
	if (itemselected != blocks::AIR){
		glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex((itemselected), 1)]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2d(mousex - 16, mousey - 16);
		glTexCoord2f(1.0, 0.0);
		glVertex2d(mousex + 16, mousey - 16);
		glTexCoord2f(1.0, 1.0);
		glVertex2d(mousex + 16, mousey + 16);
		glTexCoord2f(0.0, 1.0);
		glVertex2d(mousex - 16, mousey + 16);
		glEnd();
		glprint((int)mousex + 4, (int)mousey + 16, BlockInfo(pcsselected).getBlockName());
	}
	if (inventorybox[si][sj] != 0 && sf == 1){
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glprint((int)mousex, (int)mousey - 16, BlockInfo(inventorybox[si][sj]).getBlockName());
	}
	mousebl = mouseb;
}

void additem(block itemname){
	//向背包里加入物品t
	bool f = false;
	for (int i = 0; i != 10; i++){
		if (inventorybox[3][i] == blocks::AIR){
			inventorybox[3][i] = itemname;
			inventorypcs[3][i] = 1;
			f = true;
		}
		else if (inventorybox[3][i] == itemname && inventorypcs[3][i] < 255){
			inventorypcs[3][i]++;
			f = true;
		}
		if (f) break;
	}
	if (!f){
		for (int i = 0; i != 3; i++){
			for (int j = 0; j != 10; j++){
				if (inventorybox[i][j] == blocks::AIR){
					inventorybox[i][j] = itemname;
					inventorypcs[i][j] = 1;
					f = true;
				}
				else if (inventorybox[i][j] == itemname && inventorypcs[i][j] < 255){
					inventorypcs[i][j]++;
					f = true;
				}

				if (f) break;
			}
		}
	}
}

void saveScreenshot(int x, int y, int w, int h, string filename){
	textures::TEXTURE_RGB scrBuffer;
	while (w % 4 != 0){ w -= 1; }
	while (h % 4 != 0){ h -= 1; }
	scrBuffer.sizeX = w;
	scrBuffer.sizeY = h;
	scrBuffer.buffer = shared_ptr<ubyte>((ubyte*)malloc(w*h * 3));
	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, scrBuffer.buffer.get());
	textures::SaveRGBImage(filename, scrBuffer);
}

void createThumbnail(){
	std::stringstream ss;
	ss << "\\Worlds\\" << world::worldname << "\\Thumbnail.bmp";
	saveScreenshot(0, 0, windowwidth, windowheight, ss.str());
}