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
//|Programming language FreeBASIC(Compiler Version: 1.03.0+)                  |//
//|NEWorld-CPP版 语言：C++                编译器版本：VC++ with C++11           |//
//|Programming language C++(Compiler Version VC++ with C++11)                |//
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
0.4.9_Preview_0.1 & 0.2
1. 以0.4.9_Preview_150414为基础翻译
2. 将glfw2更换成glfw3
3. 将pblocks,pbrightness换成数组

0.4.9_Preview_0.3 & 0.4(未发布)
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

0.4.9_Preview_0.5
1. 更新版本到v35(内测版)
2. 解决了/W4级别下的绝大多数警告
3. 各种优化

0.4.9_Preview_0.6(未发布)
1. 重构方块属性
2. 继续优化
3. 调整了控制台的换行
4. 删除了pickblock等函数的控制台输出
5. 去掉了注释行末的分号

0.4.9_Preview_0.7
1. 修改了一下材质加载
2. 加入猫（但是没有皮肤，所以你看不到它们）（后来被取消）
3. 修复了创建世界菜单的响应问题
4. 支持中文输入！
5. 支持任意调整窗口大小
6. 完成了所有菜单！支持完整的进入世界

0.4.9_Preview_0.8
1. 更新到VS2015
2. 优化
3. 加入了加载界面
4. 修复了一些BUG

0.4.9_Preview_0.9
1. 更新到0.4.9r4版本
2. 优化（加载、卸载区块加速）
3. 用是否使用加载界面的选项代替了是否使用ChunkIndexArray
4. 去掉了注释后的分号（上一次是【注释行】后的分号）
5. 支持retina屏幕（需手动编译）

(未翻译：shader，VBO开关，渲染器更新，骨骼，皮肤)
*/

#include "DeveloperOptions.h"
#include "Def.h"
#include "blocks.h"
#include "OpenGL.h"
#include "textures.h"
#include "renderer.h"
#include "Hitbox.h"
#include "player.h"
#include "WorldGen.h"
#include "world.h"
#include "menus.h"
#include "Skin.h"
#include "Render.h"
#include "glprinting.h"
#include "Particles.h"


void outputGameInfo();
void LoadTextures();
void loadGame();
void saveGame();
void updateThreadFunc();
void updategame(bool FirstUpdateThisFrame);
void saveScreenshot(int x, int y, int w, int h, string filename);
void createThumbnail();

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
	hMutex = CreateMutex(NULL, FALSE, L"");
	memset(player::inventorybox, 0, sizeof(player::inventorybox));
	memset(player::inventorypcs, 0, sizeof(player::inventorypcs));
	player::inventorybox[3][0] = blocks::WOOD;
	player::inventorypcs[3][0] = 32;
	//========主菜单(就怪了)========

	windowwidth = defaultwindowwidth;
	windowheight = defaultwindowheight;
	cout << "[Console][Debug] Init OpenGL... "<<glfwInit() << "/1 ";
	std::stringstream ss;
	ss << "NEWorld " << MAJOR_VERSION << MINOR_VERSION << EXT_VERSION;
	win = glfwCreateWindow(windowwidth*(retina ? 2 : 1), windowheight*(retina ? 2 : 1), ss.str().c_str(), NULL, NULL);
	glfwMakeContextCurrent(win);
	cout << glewInit() << "/0" << std::endl;
	CenterScreen();
	glfwSetInputMode(win, GLFW_STICKY_KEYS, true); //?
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(win, &scrollEvent);
	glfwSetWindowSizeCallback(win, &winsizecall);
	glfwSetCharCallback(win, &CharInputFun);
	glfwSetCursorPosCallback(win, &CurPosCall);
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

	printf("[Console][Game]");
	printf("Starting game...\n");
	//游戏开始(还没呢QAQ)
	//各种初始化ing...
	MutexLock(Mutex);
	updateThread = thread(updateThreadFunc);


	//初始化游戏状态
	printf("[Console][Game]");
	printf("Init player...\n");
	player::InitHitbox();
	player::xpos = 0;
	player::ypos = 150;
	player::zpos = 0;
	loadGame();
	player::MoveHitboxToPosition();
	player::InitPosition();
	printf("[Console][Game]");
	printf("Init world...\n");
	world::Init();
	GUIrenderswitch = true;
	//world::sortChunkRenderList((int)player::xpos, (int)player::ypos, (int)player::zpos);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	fctime = timer(); uctime = timer(); lastupdate = timer();
	setupNormalFog();
	handleLimit = 5;
	printf("[Console][Game]");
	printf("Game start!\n");
	//这才是游戏开始!
	updateThreadRun = true;
	loading = true;
	MutexUnlock(Mutex);
	do{
		//主循环，被简化成这样，惨不忍睹啊！
//		Time_overall_ = timer();

		if ((timer() - uctime) >= 1.0){
			uctime = timer();
			ups = upsc;
			upsc = 0;
		}
		if (!loading)
			drawMain();
		else
			drawLoading();
		
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == 1&&!loading){
			createThumbnail();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			setFontColor(1.0, 1.0, 1.0, 1.0);
			glprint(0, 0, "Saving world...");
			glfwSwapBuffers(win);
			glfwPollEvents();
			MutexUnlock(Mutex);
			MutexLock(Mutex);
			updateThreadRun = false;
			saveGame();
			world::destroyAllChunks();
			MutexUnlock(Mutex);
			updateThread.join();
			printf("[Console][Game]");
			printf("Returned to main menu\n");
			ESCP = true;
			goto main_menu;
		}

		//Time_overall = timer() - Time_overall;

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

void updateThreadFunc(){
	//游戏更新线程函数
	static bool FirstUpdateThisFrame = false;
	//Wait until start...
	MutexLock(Mutex);
	while (!updateThreadRun){
		//Sleep(50)
		MutexUnlock(Mutex);
		MutexLock(Mutex);
	}
	MutexUnlock(Mutex);

	//Thread starts
	MutexLock(Mutex);
	while (updateThreadRun){

		while (updateThreadPaused){
			MutexUnlock(Mutex);
			MutexLock(Mutex);
		}

//		Time_updategame = 0;
		FirstUpdateThisFrame = true;

		updateTimer = timer();
		while ((updateTimer - lastupdate) >= 1 / (double)30 && upsc < 60){
			lastupdate += 1 / (double)30;
			upsc += 1;
			updategame(FirstUpdateThisFrame);
			FirstUpdateThisFrame = false;
		}

		if ((timer() - uctime) >= 1.0) {
			uctime = timer();
			ups = upsc;
			upsc = 0;
		}
		MutexUnlock(Mutex);
		MutexLock(Mutex);

	}
	MutexUnlock(Mutex);
}

void outputGameInfo() {

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

	//Skins[SkinTexture::CAT] = textures::LoadRGBATexture("textures\\skins\\Animals\\cat.bmp", "textures\\skins\\Animals\\catmask.bmp");
}

void saveGame(){
	world::saveAllChunks();
	player::save(world::worldname);
}

void loadGame(){
	player::load(world::worldname);
}

bool isPressed(int key, bool setFalse = false) {
	static bool keyPressed[GLFW_KEY_LAST + 1];
	if (setFalse) { keyPressed[key] = false; return true; }
	if (key > GLFW_KEY_LAST || key <= 0) return false;
	if (!glfwGetKey(win, key)) keyPressed[key] = false;
	if (!keyPressed[key] && glfwGetKey(win,key)) {
		keyPressed[key] = true;
		return true;
	}
	else {
		return false;
	}
}

void updategame(bool FirstUpdateThisFrame){

//	Time_updategame_ = timer();

	static double Wprstm;
	static bool WP;

	player::BlockInHand = player::inventorybox[3][player::itemInHand];


	if (!bagOpened && !loading)
		glfwSetCursorPos(win, windowwidth / 2 * (retina ? 2 : 1), windowheight / 2 * (retina ? 2 : 1));

	//获得鼠标按下情况
	mb = 0;
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mb = 1;
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mb = 2;

	//world::unloadedChunks=0
	world::rebuiltChunks = 0;
	world::updatedChunks = 0;

	//ciArray move
	if (UseCIArray && world::ciArrayAval){
		if (world::ciArray.originX != player::cxt - viewdistance - 2 || world::ciArray.originY != player::cyt - viewdistance - 2 || world::ciArray.originZ != player::czt - viewdistance - 2){
			world::ciArray.moveTo(player::cxt - viewdistance - 2, player::cyt - viewdistance - 2, player::czt - viewdistance - 2);
		}
	}

	if (FirstUpdateThisFrame||loading){

		//卸载区块(Unload chunks)
		world::sortChunkUnloadList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
		int sumUnload;
		if (handleLimit > 0)
			sumUnload = world::chunkUnloads > handleLimit ? handleLimit : world::chunkUnloads;
		else
			sumUnload = world::chunkUnloads;

		for (int i = 1; i <= sumUnload; i++) {
			int cx = world::chunkUnloadList[i][1];
			int cy = world::chunkUnloadList[i][2];
			int cz = world::chunkUnloadList[i][3];
			world::getChunkPtr(cx, cy, cz)->Unload();
			world::DeleteChunk(cx, cy, cz);
		}

		//加载区块(Load chunks)
		world::sortChunkLoadList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
		int sumLoad;
		if (handleLimit > 0)
			sumLoad = world::chunkLoads > handleLimit ? handleLimit : world::chunkLoads;
		else
			sumLoad = world::chunkLoads;

		for (int i = 1; i <= sumLoad; i++){
			int cx = world::chunkLoadList[i][1];
			int cy = world::chunkLoadList[i][2];
			int cz = world::chunkLoadList[i][3];

			world::AddChunk(cx, cy, cz);
			world::getChunkPtr(cx, cy, cz)->Load();
			if (loading) {
				MutexUnlock(Mutex);
				MutexLock(Mutex);
			}
		}
		//if (rnd() < 0.1) world::MOs.push_back(shared_ptr<Mo>(new Cat(player::xa + rand() % 40 - 20, player::ya, player::za + rand() % 40 - 20)))
	}
	if (loading) return;
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
	double lx, ly, lz, sidedist[7];
	int sidedistmin;
	lx = player::xpos; ly = player::ypos + player::height + player::heightExt; lz = player::zpos;

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
	bool puted = false;     //标准的chinglish吧。。。主要是put已经被FB作为关键字了。。。
	if (!bagOpened) {
		//从玩家位置发射一条线段
		for (int i = 0; i < selectPrecision*selectDistance; i++) {
			//线段延伸
			lx += sin(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			ly += cos(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			lz += cos(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;

			//碰到方块
			if (BlockInfo(world::getblock(RoundInt(lx), RoundInt(ly), RoundInt(lz))).isSolid()) {
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

				sidedist[1] = abs(y + 0.5 - ly);          //顶面
				sidedist[2] = abs(y - 0.5 - ly);		  //底面
				sidedist[3] = abs(x + 0.5 - lx);		  //左面
				sidedist[4] = abs(x - 0.5 - lx);	   	  //右面
				sidedist[5] = abs(z + 0.5 - lz);		  //前面
				sidedist[6] = abs(z - 0.5 - lz);		  //后面
				sidedistmin = 1;						  //离哪个面最近
				for (int j = 2; j <= 6; j++) {
					if (sidedist[j] < sidedist[sidedistmin]) sidedistmin = j;
				}

				if (world::chunkOutOfBound(selcx, selcy, selcz) == false) {
					selb = world::getChunkPtr(selcx, selcy, selcz)->getblock(selbx, selby, selbz);
				}

				switch (sidedistmin) {
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

				if (mb == 1 || glfwGetKey(win,GLFW_KEY_ENTER)==GLFW_PRESS) {
					particles::throwParticle(world::getblock(x, y, z),
						float(x + rnd() - 0.5f), float(y + rnd() - 0.2f), float(z + rnd() - 0.5f),
						float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f),
						float(rnd()*0.01f + 0.02f), int(rnd() * 30) + 30);

					if (selx != oldselx || sely != oldsely || selz != oldselz)
						seldes = 0.0;
					else
						seldes += 5.0;
					if (seldes >= 100.0) {
						player::additem(world::getblock(x, y, z));
						for (int j = 1; j <= 25; j++) {
							particles::throwParticle(world::getblock(x, y, z),
								float(x + rnd() - 0.5f), float(y + rnd() - 0.2f), float(z + rnd() - 0.5f),
								float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f), float(rnd()*0.2f - 0.1f),
								float(rnd()*0.02 + 0.03), int(rnd() * 60) + 30);
						}
						world::pickblock(x, y, z);
					}
				}
				//放置方块
				if ((mb == 2 && mbp == false) || isPressed(GLFW_KEY_TAB) && player::inventorypcs[3][player::itemInHand] > 0) {
					puted = true;
					switch (sidedistmin) {
					case 1:
						if (player::putblock(x, y + 1, z, player::BlockInHand) == false) puted = false;
						break;
					case 2:
						if (player::putblock(x, y - 1, z, player::BlockInHand) == false) puted = false;
						break;
					case 3:
						if (player::putblock(x + 1, y, z, player::BlockInHand) == false) puted = false;
						break;
					case 4:
						if (player::putblock(x - 1, y, z, player::BlockInHand) == false) puted = false;
						break;
					case 5:
						if (player::putblock(x, y, z + 1, player::BlockInHand) == false) puted = false;
						break;
					case 6:
						if (player::putblock(x, y, z - 1, player::BlockInHand) == false) puted = false;
						break;
					}
					if (puted) {
						player::inventorypcs[3][player::itemInHand]--;
						if (player::inventorypcs[3][player::itemInHand] == 0) player::inventorypcs[3][player::itemInHand] = blocks::AIR;
					}
				}
				break;
			}
		}

		if (selx != oldselx || sely != oldsely || selz != oldselz || (mb == 0 && glfwGetKey(win, GLFW_KEY_ENTER) != GLFW_PRESS)) seldes = 0.0;
		oldselx = selx;
		oldsely = sely;
		oldselz = selz;

		player::intxpos = RoundInt(player::xpos);
		player::intypos = RoundInt(player::ypos);
		player::intzpos = RoundInt(player::zpos);

		//移动！(生命在于运动)
		if (glfwGetKey(win, GLFW_KEY_W) == 1) {
			if (!WP) {
				if (Wprstm == 0.0) {
					Wprstm = timer();
				}
				else {
					if (timer() - Wprstm <= 0.5) { player::Running = true; Wprstm = 0.0; }
					else Wprstm = timer();
				}
			}
			if (Wprstm != 0.0 && timer() - Wprstm > 0.5) Wprstm = 0.0;
			WP = true;
			player::xa = -sin(player::heading*M_PI / (double)180) * player::speed;
			player::za = -cos(player::heading*M_PI / (double)180) * player::speed;
		}
		else {
			player::Running = false;
			WP = false;
		}
		if (player::Running) {
			player::speed = runspeed;
			if (FOVyExt < 9.8)
				FOVyExt = 10.0f - (10.0f - FOVyExt)*0.8f;
			else
				FOVyExt = 10.0f;
		}
		else {
			player::speed = walkspeed;
			if (FOVyExt > 0.2)
				FOVyExt *= 0.8f;
			else
				FOVyExt = 0.0f;
		}

		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
			player::xa = sin(player::heading*M_PI / (double)180) * player::speed;
			player::za = cos(player::heading*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
			player::xa = sin((player::heading - 90)*M_PI / (double)180) * player::speed;
			player::za = cos((player::heading - 90)*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
			player::xa = -sin((player::heading - 90)*M_PI / (double)180) * player::speed;
			player::za = -cos((player::heading - 90)*M_PI / (double)180) * player::speed;
			Wprstm = 0.0;
		}

		if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
			if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				player::xa = -sin(player::heading*M_PI / (double)180) * runspeed * 10;
				player::za = -cos(player::heading*M_PI / (double)180) * runspeed * 10;
			}
			else {
				player::xa = sin(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) * runspeed * 20;
				player::ya = cos(M_PI / 180 * (player::lookupdown + 90)) * runspeed * 20;
				player::za = cos(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) * runspeed * 20;
			}
		}

		if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
			player::xa = sin(player::heading*M_PI / (double)180) * runspeed * 10;
			player::za = cos(player::heading*M_PI / (double)180) * runspeed * 10;
		}

		//切换方块
		if (isPressed(GLFW_KEY_Z) && player::itemInHand > 0) player::itemInHand--;
		if (isPressed(GLFW_KEY_X) && player::itemInHand < 9) player::itemInHand++;

		mwl = mw;

		//转头！你治好了我多年的颈椎病！

		if (mx > int(windowwidth / 2))
			player::heading -= (mx - int(windowwidth / 2))*mousemove;

		if (mx < int(windowwidth / 2))
			player::heading += (int(windowwidth / 2) - mx)*mousemove;

		if (my < int(windowheight / 2))
			player::lookupdown -= (int(windowheight / 2) - my)*mousemove;

		if (my > windowheight / 2)
			player::lookupdown += (my - int(windowheight / 2))*mousemove;


		if (glfwGetKey(win, GLFW_KEY_RIGHT) == 1) {
			player::xlookspeed -= mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT) == 1) {
			player::xlookspeed += mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_UP) == 1) {
			player::ylookspeed -= mousemove * 4;
		}
		if (glfwGetKey(win, GLFW_KEY_DOWN) == 1) {
			player::ylookspeed += mousemove * 4;
		}

		player::heading += player::xlookspeed;
		player::lookupdown += player::ylookspeed;
		player::xlookspeed *= 0.8f;
		player::ylookspeed *= 0.8f;

		//限制角度，别把头转掉下来了 ←_←
		if (player::lookupdown < -90) player::lookupdown = -90;
		if (player::lookupdown > 90) player::lookupdown = 90;

		//起跳！
		if (isPressed(GLFW_KEY_SPACE)){
			if (!player::inWater) {
				if ((player::OnGround || player::AirJumps < MaxAirJumps) && FLY == false && CROSS == false){
					if (player::OnGround == false) player::AirJumps++;
					player::jump = 0.4; player::OnGround = false;
				}
				if (FLY || CROSS) { player::ya += walkspeed; isPressed(GLFW_KEY_SPACE, true); }
				Wprstm = 0.0;
			}
			else{
				player::ya = walkspeed;
				isPressed(GLFW_KEY_SPACE, true);
			}
		}

		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
			if (CROSS || FLY){
				player::ya -= walkspeed;
			}
			else{
				if (player::heightExt > -0.59f)  player::heightExt -= 0.1f; else player::heightExt = -0.6f;
			}
			Wprstm = 0.0;
		}

		//各种设置切换

		if (isPressed(GLFW_KEY_F1)){
			FLY = !FLY;
			player::jump = 0.0;
		}

		if (isPressed(GLFW_KEY_F2)) shouldGetScreenshot = true;

		if (isPressed(GLFW_KEY_F3)) DEBUGMODE = !DEBUGMODE;


		if (isPressed(GLFW_KEY_F3) && glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS){
			DebugHitbox = !DebugHitbox;
			DEBUGMODE = true;
		}

		if (isPressed(GLFW_KEY_F4) == GLFW_PRESS) CROSS = !CROSS;

		if (isPressed(GLFW_KEY_F5) == GLFW_PRESS) GUIrenderswitch = !GUIrenderswitch;

	}

	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS && EP == false){
		bagOpened = !bagOpened;
		if (!bagOpened){
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else{
			shouldGetThumbnail = true;
		}
		EP = true;
	}
	if (glfwGetKey(win, GLFW_KEY_E) != GLFW_PRESS) EP = false;

	if (!bagOpened){
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS) ESCP = false;

		if (isPressed(GLFW_KEY_L)) world::saveAllChunks();
	}
	//跳跃
	if (!player::inWater){
		if (FLY == false && CROSS == false){

			player::ya = -0.001;

			if (player::OnGround){
				player::jump = 0.0;
				player::AirJumps = 0;
				isPressed(GLFW_KEY_SPACE, true);
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
		isPressed(GLFW_KEY_SPACE, true);
		if (player::ya <= 0.001){
			player::ya = -0.001;
			if (!player::OnGround) player::ya = -0.1;
		}

	}

	//if (player::NearWall && FLY == false && CROSS == false){
	//	player::ya += walkspeed
	//	player::jump = 0.0
	//  //爬墙
	//}

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

//	Time_updategame += timer() - Time_updategame;

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
	ss << "Worlds\\" << world::worldname << "\\Thumbnail.bmp";
	saveScreenshot(0, 0, windowwidth, windowheight, ss.str());
}
