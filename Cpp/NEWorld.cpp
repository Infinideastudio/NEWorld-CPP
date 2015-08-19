//====================<----------NEWorld-NE����---------->======================//
//|�����ߣ���չ��[qiaozhanrong]                                                |//
//|Developed by Qiao Zhanrong                                                 |//
//|�ٷ���վ��http://www.neworldsite.icoc.cc/                                   |//
//|Official website http://www.neworldsite.icoc.cc/                           |//
//|�ٶ����ɣ�http://tieba.baidu.com/p/2822071396?pn=1                          |//
//|BaiduTieba topic address http://tieba.baidu.com/p/2822071396?pn=1          |//
//|��ַ���й�-����-�Ͼ�-������-����¥-�Ĳ���2��                                   |//
//|Wenchang street No.2,Nanjing,Jiangsu Province,P.R.C.                        |//
//|��ַ2���й�-����-�Ͼ�-������-�Ͼ�ʦ����ѧ����Сѧ[����¥У��]-4¥��(4)��        |//
//|Class 4, Grade 6, The Primary School Attached To Nanjing Normal University |//
//|�����벻����QAQ ( My English is not very good ... Don't mind ! )            |//
//|----------------------------------------------------------------------------|//
//|NEWolrdԭ��   ���ԣ�FreeBASIC          �������汾��FBC 1.03.0 ������          |//
//|Programming language FreeBASIC(Compiler Version: 1.03.0+)                  |//
//|NEWorld-CPP�� ���ԣ�C++                �������汾��VC++ with C++11           |//
//|Programming language C++(Compiler Version VC++ with C++11)                |//
//|�汾��0.4.9 [Alpha]                                                        |//
//|Game version: Alpha 0.4.9                                                   |//
//==============================================================================//
//��Minecraft����Markus Persson�¾���
//THANKS A LOT to the author Markus Persson Minecraft!
//Ϊ�й���IT��ҵ�������ף�
//Make a contribution to the China IT career!
//==============================================================================//
//
//
//==============================   Initialize   ================================//
//==============================��ʼ��(��������)================================//

/*

NEWorld-CPP��һ�����ߣ��������ߣ���Null (abc612008)

���¼�¼��
0.4.9_Preview_0.1 & 0.2
1. ��0.4.9_Preview_150414Ϊ��������
2. ��glfw2������glfw3
3. ��pblocks,pbrightness��������

0.4.9_Preview_0.3 & 0.4(δ����)
1. ���°汾��0.4.9_Preview_150713
2. ����chunk�����ʽ����FB���ͼ���ټ��ݣ�
3. ���������0����
4. �����������鱣���ʽ����FB���ͼ���ټ��ݣ�
5. �Ż���renderer::init,world::getblock�Ⱥ���
6. Frustum�Ż�
7. ���������Ż�
8. �޸���ʰȡ����
9. �޸���chunk���ط�Χ����
10. ѡ�������ɣ�������Ϸ�Ľ�����BUG

0.4.9_Preview_0.5
1. ���°汾��v35(�ڲ��)
2. �����/W4�����µľ����������
3. �����Ż�

0.4.9_Preview_0.6(δ����)
1. �ع���������
2. �����Ż�
3. �����˿���̨�Ļ���
4. ɾ����pickblock�Ⱥ����Ŀ���̨���
5. ȥ����ע����ĩ�ķֺ�

0.4.9_Preview_0.7
1. �޸���һ�²��ʼ���
2. ����è������û��Ƥ���������㿴�������ǣ���������ȡ����
3. �޸��˴�������˵�����Ӧ����
4. ֧���������룡
5. ֧������������ڴ�С
6. ��������в˵���֧�������Ľ�������

0.4.9_Preview_0.8
1. ���µ�VS2015
2. �Ż�
3. �����˼��ؽ���
4. �޸���һЩBUG

0.4.9_Preview_0.9
1. ���µ�0.4.9r4�汾
2. �Ż������ء�ж��������٣�
3. ���Ƿ�ʹ�ü��ؽ����ѡ��������Ƿ�ʹ��ChunkIndexArray
4. ȥ����ע�ͺ�ķֺţ���һ���ǡ�ע���С���ķֺţ�
5. ֧��retina��Ļ�����ֶ����룩

(δ���룺shader��VBO���أ���Ⱦ�����£�������Ƥ��)
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
//==============================     ������     ================================//

//�����뿴qiaozhanrongͯЬ������
int main(){
	//......
	//......
	//......
	//......"˳��˵һ�£�GL�и���˼��Ůͬ������OpenGL��������ľ��ǡ�����"
	//......
	//......
	//......
	//......"����ʽŮͬ����(���˸���ȥ��)!"
	//......
	//......
	//......
	hMutex = CreateMutex(NULL, FALSE, L"");
	memset(player::inventorybox, 0, sizeof(player::inventorybox));
	memset(player::inventorypcs, 0, sizeof(player::inventorypcs));
	player::inventorybox[3][0] = blocks::WOOD;
	player::inventorypcs[3][0] = 32;
	//========���˵�(�͹���)========

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

	//����Ч��
	glDisable(GL_CULL_FACE);

	splashscreen();

	InitGL();

	LoadTextures();

	//���˵�
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
	//��Ϸ��ʼ(��û��QAQ)
	//���ֳ�ʼ��ing...
	MutexLock(Mutex);
	updateThread = thread(updateThreadFunc);


	//��ʼ����Ϸ״̬
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
	//�������Ϸ��ʼ!
	updateThreadRun = true;
	loading = true;
	MutexUnlock(Mutex);
	do{
		//��ѭ�������򻯳��������Ҳ��̶ð���
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
	//��������ɾ��Ҳû��ϵ ��_�����²�FB��glfw�У�
	glfwTerminate();
	return 0;
}

void updateThreadFunc(){
	//��Ϸ�����̺߳���
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
	printf("�� �´��޼ʹ����һ�ӭ���ļ��룡��            \n ");
	printf("�� QQȺ�ţ�434811287          ��            \n");
	printf("[�ⶼ���㷢���ˡ�����]          \n");

	//color 7,0
	//view printf 5 to 23
	//cls

}

void LoadTextures(){
	//��������
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

	//�����갴�����
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

		//ж������(Unload chunks)
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

		//��������(Load chunks)
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
	//���ض���
	for (int i = 0; i != world::loadedChunks; i++){
		world::chunk* cp = &world::chunks[i];
		if (cp->loadAnim <= 0.3f)
			cp->loadAnim = 0.0f;
		else
			cp->loadAnim *= 0.6f;
	}
	//���״̬����
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
	//�ж�ѡ�еķ���
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
	bool puted = false;     //��׼��chinglish�ɡ�������Ҫ��put�Ѿ���FB��Ϊ�ؼ����ˡ�����
	if (!bagOpened) {
		//�����λ�÷���һ���߶�
		for (int i = 0; i < selectPrecision*selectDistance; i++) {
			//�߶�����
			lx += sin(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			ly += cos(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;
			lz += cos(M_PI / 180 * (player::heading - 180))*sin(M_PI / 180 * (player::lookupdown + 90)) / (double)selectPrecision;

			//��������
			if (BlockInfo(world::getblock(RoundInt(lx), RoundInt(ly), RoundInt(lz))).isSolid()) {
				int x, y, z;
				x = RoundInt(lx);
				y = RoundInt(ly);
				z = RoundInt(lz);

				selx = x; sely = y; selz = z;

				//�ҷ����������鼰λ��
				selcx = world::getchunkpos(x);
				selbx = world::getblockpos(x);
				selcy = world::getchunkpos(y);
				selby = world::getblockpos(y);
				selcz = world::getchunkpos(z);
				selbz = world::getblockpos(z);

				sidedist[1] = abs(y + 0.5 - ly);          //����
				sidedist[2] = abs(y - 0.5 - ly);		  //����
				sidedist[3] = abs(x + 0.5 - lx);		  //����
				sidedist[4] = abs(x - 0.5 - lx);	   	  //����
				sidedist[5] = abs(z + 0.5 - lz);		  //ǰ��
				sidedist[6] = abs(z - 0.5 - lz);		  //����
				sidedistmin = 1;						  //���ĸ������
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
				//���÷���
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

		//�ƶ���(���������˶�)
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

		//�л�����
		if (isPressed(GLFW_KEY_Z) && player::itemInHand > 0) player::itemInHand--;
		if (isPressed(GLFW_KEY_X) && player::itemInHand < 9) player::itemInHand++;

		mwl = mw;

		//תͷ�����κ����Ҷ���ľ�׵����

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

		//���ƽǶȣ����ͷת�������� ��_��
		if (player::lookupdown < -90) player::lookupdown = -90;
		if (player::lookupdown > 90) player::lookupdown = 90;

		//������
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

		//���������л�

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
	//��Ծ
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
	//  //��ǽ
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
