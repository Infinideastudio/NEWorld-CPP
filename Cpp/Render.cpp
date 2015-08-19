#include "Render.h"
#include "OpenGL.h"
#include "glprinting.h"
#include "Player.h"
#include "frustum.h"
#include "Textures.h"
#include "GUI.h"
#include "Particles.h"

int renderedChunk = 0;

int selx, sely, selz, oldselx, oldsely, oldselz;
float selt, seldes;
block selb;
brightness selbr;
int selbx, selby, selbz, selcx, selcy, selcz;

int fps;                               //帧速率
int fpsc;                              //帧数计数
double fctime;
int ups;
int upsc;
double uctime;
bool GUIrenderswitch;
bool DEBUGMODE;                  //调试模式
bool DebugHitbox;            //Hitbox调试模式
bool DebugChunk;             //区块调试模式[Not enabled!]
bool gamebegin;
bool loading;
bool bagOpened;

bool shouldGetScreenshot, shouldGetThumbnail;

TextureID splTex;
TextureID BlockTexture[20];
TextureID BlockTextures;
TextureID guiImage[6];
TextureID DestroyImage[11];
TextureID Skins[2];

void saveScreenshot(int x,int y, int w, int h, string filename);
void createThumbnail();

void debugText(string s, bool init = false) {
	static int pos = 0;
	if (init) {
		pos = 0;
		return;
	}
	glprint(0, 16 * pos, s);
	pos++;
}

void drawMain() {
	//画场景
//	Time_renderscene_ = timer();
	double xpos = player::xpos;
	double ypos = player::ypos + player::height + player::heightExt;
	double zpos = player::zpos;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVyNormal + FOVyExt, windowwidth / (double)windowheight, 0.1, 512.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(player::lookupdown, 1, 0, 0);
	glRotated(360.0 - player::heading, 0, 1, 0);
	glTranslated(-xpos, -ypos, -zpos);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	player::cxt = world::getchunkpos((int)player::xpos);
	player::cyt = world::getchunkpos((int)player::ypos);
	player::czt = world::getchunkpos((int)player::zpos);

	//更新区块显示列表
	world::sortChunkRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
	for (int i = 1; i <= world::chunkRenders; i++) {
		auto cptr = world::chunkRenderList[i].second;
		cptr->buildlists();
	}

	//删除已卸载区块的显示列表
	for (int i = 0; i != 3; i++) {
		if (world::displayListUnloadList.size() <= 0) break;
		glDeleteLists(world::displayListUnloadList[world::displayListUnloadList.size() - 1], 3);
		world::displayListUnloadList.pop_back();
	}
	//if ((timer() - uctime) >= 1.0) {
	//	uctime = timer()
	//	ups = upsc
	//	upsc = 0
	//}
	double plookupdown = player::lookupdown;
	double pheading = player::heading;
	glLoadIdentity();
	glRotated(plookupdown, 1, 0, 0);
	glRotated(360.0 - pheading, 0, 1, 0);
	Frustum::calc();
	vector<RenderChunk> displayChunks;
	for (int i = 0; i != world::loadedChunks; i++) {
		if (world::chunks[i].list > 0 && world::chunkInRange(world::chunks[i].cx, world::chunks[i].cy, world::chunks[i].cz, player::cxt, player::cyt, player::czt, viewdistance)) {
			if (Frustum::aabbInFrustum(world::chunks[i].getRelativeAABB(xpos, ypos, zpos))) displayChunks.push_back(&world::chunks[i]);
		}
	}
	MutexUnlock(Mutex);
	renderedChunk = displayChunks.size();
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		glCallList(cr.list);
		glTranslated(-cr.cx * 16 + xpos, -cr.cy * 16 + cr.loadAnim + ypos, -cr.cz * 16 + zpos);
	}
	if (seldes > 0.0) {
		glTranslated(selx - xpos, sely - ypos, selz - zpos);
		renderDestroy(seldes, 0, 0, 0);
		glTranslated(-selx + xpos, -sely + ypos, -selz + zpos);
	}
	MutexLock(Mutex);
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	particles::renderall();
	MutexUnlock(Mutex);
	glDisable(GL_TEXTURE_2D);
	glTranslated(selx - xpos, sely - ypos, selz - zpos);
	if (GUIrenderswitch) drawborder(0, 0, 0);
	glTranslated(-selx + xpos, -sely + ypos, -selz + zpos);
	//for (unsigned int i = 0; i != world::MOs.size() i++) world::MOs[i]->renderer()
	glLoadIdentity();
	glRotated(plookupdown, 1, 0, 0);
	glRotated(360.0 - pheading, 0, 1, 0);
	glTranslated(-xpos, -ypos, -zpos);
	glLoadIdentity();
	glRotated(plookupdown, 1, 0, 0);
	glRotated(360.0 - pheading, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		glCallList(cr.list + 1);
		glTranslated(-cr.cx * 16 + xpos, -cr.cy * 16 + cr.loadAnim + ypos, -cr.cz * 16 + zpos);
	}
	glDisable(GL_CULL_FACE);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		glCallList(cr.list + 2);
		glTranslated(-cr.cx * 16 + xpos, -cr.cy * 16 + cr.loadAnim + ypos, -cr.cz * 16 + zpos);
	}

	glLoadIdentity();
	glRotated(player::lookupdown, 1, 0, 0);
	glRotated(360.0 - player::heading, 0, 1, 0);
	glTranslated(-xpos, -ypos, -zpos);
	MutexLock(Mutex);
	glDisable(GL_TEXTURE_2D);
	drawcloud(player::xpos, player::zpos);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//MutexLock(Mutex)

	//Time_renderscene = timer() - Time_renderscene;
	//Time_renderGUI_ = timer();

	if (GUIrenderswitch) {
		drawGUI();
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	if (world::getblock((int)player::xpos, int(player::ypos + player::height + player::heightExt), (int)player::zpos) == blocks::WATER) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex(blocks::WATER, 1)]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2i(0, 0);
		glTexCoord2f(0.0, 0.0); glVertex2i(0, windowheight);
		glTexCoord2f(1.0, 0.0); glVertex2i(windowwidth, windowheight);
		glTexCoord2f(1.0, 1.0); glVertex2i(windowwidth, 0);
		glEnd();
	}

	if (bagOpened)
		drawBag();

	if (shouldGetScreenshot) {
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
	if (shouldGetThumbnail) {
		shouldGetThumbnail = false;
		createThumbnail();
	}

	//屏幕刷新，千万别删，后果自负！！！
	//====refresh====//
	MutexUnlock(Mutex);
	glfwSwapBuffers(win);
	glfwPollEvents();
	MutexLock(Mutex);
	//==refresh end==//

	//Time_screensync = timer() - Time_screensync;

}

void drawborder(int x, int y, int z) {
	//绘制选择边框，建议用GL_LINE_LOOP，别学我QAQ
	static float extrize = 0.002f; //实际上这个边框应该比方块大一些，否则很难看
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
void splashscreen() {

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
	for (i = 0; i != 256; i += 4) {
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
		glprint(400, 200, MAJOR_VERSION + MINOR_VERSION);
		//glfwSleep(0.001)
		Sleep(1);
	}
	glprint(10, 10, "Loading Textures...");
	glprint(10, 26, "Please Wait...");
	glfwSwapBuffers(win);
}
void drawGUI() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	int seldes_100 = int(seldes / 100);
	if (DEBUGMODE) {

		if (selb != blocks::AIR) {

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
		else {
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

	if (seldes>0.0) {

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
	for (int i = 0; i != 10; i++) {
		if (i == player::itemInHand)
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
		if (player::inventorybox[3][i] != blocks::AIR) {
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex(player::inventorybox[3][i], 1)]);
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
			ss << (int)player::inventorypcs[3][i];
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

	if (DEBUGMODE) {
		setFontColor(1.0f, 1.0f, 1.0f, 0.9f);
		std::stringstream ss;
		ss << "NEWorld v: " << VERSION << " [OpenGL " << GLVersionMajor << "." << GLVersionMinor << "|" << GLVersionRev << "]";
		debugText(ss.str()); ss.str("");
		ss << "Flying: " << boolstr(FLY);
		debugText(ss.str()); ss.str("");
		ss << "Debug Mode: " << boolstr(DEBUGMODE);
		debugText(ss.str()); ss.str("");
		ss << "Crosswall: " << boolstr(CROSS);
		debugText(ss.str()); ss.str("");
		ss << "Block: " << BlockInfo(player::BlockInHand).getBlockName() << " (ID" << (int)player::BlockInHand << ")";
		debugText(ss.str()); ss.str("");
		ss << "Fps: " << fps;
		debugText(ss.str()); ss.str("");
		ss << "Ups(Tps): " << ups;
		debugText(ss.str()); ss.str("");

		ss << "Xpos: " << player::xpos;
		debugText(ss.str()); ss.str("");
		ss << "Ypos: " << player::ypos;
		debugText(ss.str()); ss.str("");
		ss << "Zpos: " << player::zpos;
		debugText(ss.str()); ss.str("");
		ss << "Direction: " << player::heading;
		debugText(ss.str()); ss.str("");
		ss << "Head: " << player::lookupdown;
		debugText(ss.str()); ss.str("");
		ss << "On ground: " << boolstr(player::OnGround);
		debugText(ss.str()); ss.str("");
		ss << "Jump speed: " << player::jump;
		debugText(ss.str()); ss.str("");
		ss << "Near wall: " << boolstr(player::NearWall);
		debugText(ss.str()); ss.str("");
		ss << "In water: " << boolstr(player::inWater);
		debugText(ss.str()); ss.str("");

		ss << world::loadedChunks << " / " << world::allocChunks << " chunks loaded";
		debugText(ss.str()); ss.str("");
		ss << renderedChunk << " chunks rendered";
		debugText(ss.str()); ss.str("");
		ss << world::unloadedChunks << " chunks unloaded";
		debugText(ss.str()); ss.str("");
		ss << world::updatedChunks << " chunks updated";
		debugText(ss.str()); ss.str("");

		ss << "ChunkIndexCache Index = " << world::ciCacheIndex;
		debugText(ss.str()); ss.str("");

		//ss << "MO count: " << world::MOs.size()
		//debugText(ss.str()) ss.str("")

		//ss << "Particle count: " & particles::ptcsrendered & "/" & particles::ptcs.size()
		//debugText(ss.str())

		debugText("", true);
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


		//setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glDisable(GL_TEXTURE_2D);
		//glBegin(GL_QUADS);
		//glColor4f(gui::FgR, gui::FgG, gui::FgB, 0.7f);

		//glVertex2d(windowwidth - 300, 16 * 9 - 1);
		//glVertex2d(windowwidth - 300, 16 * 10 - 1);
		//glVertex2d(windowwidth - 300 + Time_renderscene / (double)Time_overall * 300, 16 * 10 - 1);
		//glVertex2d(windowwidth - 300 + Time_renderscene / (double)Time_overall * 300, 16 * 9 - 1);

		//glVertex2d(windowwidth - 300, 16 * 10 - 1);
		//glVertex2d(windowwidth - 300, 16 * 11 - 1);
		//glVertex2d(windowwidth - 300 + Time_renderGUI / (double)Time_overall * 300, 16 * 11 - 1);
		//glVertex2d(windowwidth - 300 + Time_renderGUI / (double)Time_overall * 300, 16 * 10 - 1);

		//glVertex2d(windowwidth - 300, 16 * 11 - 1);
		//glVertex2d(windowwidth - 300, 16 * 12 - 1);
		//glVertex2d(windowwidth - 300 + Time_updategame / (double)Time_overall * 300, 16 * 12 - 1);
		//glVertex2d(windowwidth - 300 + Time_updategame / (double)Time_overall * 300, 16 * 11 - 1);

		//glVertex2d(windowwidth - 300, 16 * 12 - 1);
		//glVertex2d(windowwidth - 300, 16 * 13 - 1);
		//glVertex2d(windowwidth - 300 + Time_screensync / (double)Time_overall * 300, 16 * 13 - 1);
		//glVertex2d(windowwidth - 300 + Time_screensync / (double)Time_overall * 300, 16 * 12 - 1);
		//glEnd();
		//glBegin(GL_LINE_LOOP);
		//glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
		//glVertex2d(windowwidth, 16 * 9 - 1);
		//glVertex2d(windowwidth, 16 * 10 - 1);
		//glVertex2d(windowwidth - 300, 16 * 10 - 1);
		//glVertex2d(windowwidth - 300, 16 * 9 - 1);
		//glEnd();
		//glBegin(GL_LINE_LOOP);
		//glVertex2d(windowwidth, 16 * 10 - 1);
		//glVertex2d(windowwidth, 16 * 11 - 1);
		//glVertex2d(windowwidth - 300, 16 * 11 - 1);
		//glVertex2d(windowwidth - 300, 16 * 10 - 1);
		//glEnd();
		//glBegin(GL_LINE_LOOP);
		//glVertex2d(windowwidth, 16 * 11 - 1);
		//glVertex2d(windowwidth, 16 * 12 - 1);
		//glVertex2d(windowwidth - 300, 16 * 12 - 1);
		//glVertex2d(windowwidth - 300, 16 * 11 - 1);
		//glEnd();
		//glBegin(GL_LINE_LOOP);
		//glVertex2d(windowwidth, 16 * 12 - 1);
		//glVertex2d(windowwidth, 16 * 13 - 1);
		//glVertex2d(windowwidth - 300, 16 * 13 - 1);
		//glVertex2d(windowwidth - 300, 16 * 12 - 1);
		//glEnd();

		//glEnable(GL_TEXTURE_2D);
		//setFontColor(0.0f, 0.0f, 0.0f, 0.3f);
		//glprint(windowwidth - 300, 16 * 9, "Render scene");
		//glprint(windowwidth - 300, 16 * 10, "Render GUI");
		//glprint(windowwidth - 300, 16 * 11, "Update game");
		//glprint(windowwidth - 300, 16 * 12, "Others");
		//setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else {

		setFontColor(1.0f, 1.0f, 1.0f, 0.9f);
		std::stringstream ss;
		ss << "v" << VERSION;
		glprint(0, 0, ss.str());

	}

	//检测帧速率
	if (timer() - fctime >= 1) {
		fps = fpsc;
		fpsc = 0;
		fctime = timer();
	}
	fpsc++;
	//Time_renderGUI = timer() - Time_renderGUI;
	//Time_screensync_ = timer();
}

void drawLoading() {
	MutexLock(Mutex);
	int loadedChunks = world::loadedChunks;
	world::sortChunkRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
	for (int i = 1; i <= world::chunkRenders; i++) {
		auto cptr = world::chunkRenderList[i].second;
		cptr->buildlists();
	}
	MutexUnlock(Mutex);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	if (loadedChunks == 1728 || !useLoading) {   //加载完毕
		handleLimit = 2;
		glClearColor(skycolorR, skycolorG, skycolorB, 1.0);
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		loading = false;
		MutexLock(Mutex);
		return;
	}
	float col = loadedChunks / 1728.0f;
	glClearColor(col, col, col, 1);
	setFontColor(1, 1, 1, 1);
	glprint(windowwidth / 2 - 50, windowheight / 2 - 50 + 16 * 1, "Loading...");
	glprint(windowwidth / 2 - 50, windowheight / 2 - 50 + 16 * 3, "Loading chunks!");
	char strLoadedChunks[12];
	_itoa_s(loadedChunks, strLoadedChunks, 10);
	glprint(windowwidth / 2 - 50, windowheight / 2 - 50 + 16 * 5, "Processing: " + string(strLoadedChunks));
	glfwSwapBuffers(win);
	glfwPollEvents();
}
void drawcloud(double px, double pz) {

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

	if (!generated) {

		for (i = 0; i != 128; i++) {
			for (j = 0; j != 128; j++) {
				world::cloud[i][j] = sbyte(rnd() * 2);
			}
		}

		generated = true;
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);

	for (i = 0; i != 128; i++) {
		for (j = 0; j != 128; j++) {
			int x, z;
			int cloudx, cloudz;
			x = cx + i;
			z = cz + j;
			if (x < 0)
				cloudx = 127 + (x % 128);
			else
				cloudx = x % 128;

			if (z - d < 0)
				cloudz = 127 + ((z - d) % 128);
			else
				cloudz = (z - d) % 128;

			if (world::cloud[cloudx][cloudz]) {
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
void renderDestroy(float level, int x, int y, int z) {

	//float colors
	static float ES = 0.002f;

	glColor4f(1.0, 1.0, 1.0, 1.0);

	if (level < 100.0) {
		glBindTexture(GL_TEXTURE_2D, DestroyImage[int(level / 10) + 1]);
	}
	else {
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

void drawBag() {
	//背包界面
	static int si, sj, sf;
	int leftp = (windowwidth - 392) / 2;
	int upp = windowheight - 152 - 16;
	static int mousew, mouseb, mousebl;
	static block itemselected = blocks::AIR;
	static block pcsselected = 0;
	glClearColor(skycolorR, skycolorG, skycolorB, 1.0);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
	for (int i = 0; i != 4; i++) {
		for (int j = 0; j != 10; j++) {
			if (mx >= j*(32 + 8) + leftp && mx <= j*(32 + 8) + 32 + leftp &&
				my >= i*(32 + 8) + upp && my <= i*(32 + 8) + 32 + upp) {
				si = i; sj = j; sf = 1;
				glBindTexture(GL_TEXTURE_2D, guiImage[2]);
				if (mousebl == 0 && mouseb == 1 && itemselected == player::inventorybox[i][j]) {
					if (player::inventorypcs[i][j] + pcsselected <= 255) {
						player::inventorypcs[i][j] += pcsselected;
						pcsselected = 0;
					}
					else
					{
						pcsselected = player::inventorypcs[i][j] + pcsselected - 255;
						player::inventorypcs[i][j] = 255;
					}
				}
				if (mousebl == 0 && mouseb == 1 && itemselected != player::inventorybox[i][j]) {
					std::swap(pcsselected, player::inventorypcs[i][j]);
					std::swap(itemselected, player::inventorybox[i][j]);
				}
				if (mousebl == 0 && mouseb == 2 && itemselected == player::inventorybox[i][j] && player::inventorypcs[i][j] < 255) {
					pcsselected--;
					player::inventorypcs[i][j]++;
				}
				if (mousebl == 0 && mouseb == 2 && player::inventorybox[i][j] == blocks::AIR) {
					pcsselected--;
					player::inventorypcs[i][j] = 1;
					player::inventorybox[i][j] = itemselected;
				}

				if (pcsselected == 0) itemselected = blocks::AIR;
				if (itemselected == blocks::AIR) pcsselected = 0;
				if (player::inventorypcs[i][j] == 0) player::inventorybox[i][j] = blocks::AIR;
				if (player::inventorybox[i][j] == blocks::AIR) player::inventorypcs[i][j] = 0;
			}
			else {
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
			if (player::inventorybox[i][j] != blocks::AIR) {
				glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex((player::inventorybox[i][j]), 1)]);
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
				ss << (int)player::inventorypcs[i][j];
				glprint(j*(32 + 8) + 8 + leftp, (i*(32 + 16) + 8 + upp), ss.str());
			}
		}
	}
	if (itemselected != blocks::AIR) {
		glBindTexture(GL_TEXTURE_2D, BlockTexture[textures::getTextureIndex((itemselected), 1)]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2d(mx - 16, my - 16);
		glTexCoord2f(1.0, 0.0);
		glVertex2d(mx + 16, my - 16);
		glTexCoord2f(1.0, 1.0);
		glVertex2d(mx + 16, my + 16);
		glTexCoord2f(0.0, 1.0);
		glVertex2d(mx - 16, my + 16);
		glEnd();
		glprint((int)mx + 4, (int)my + 16, BlockInfo(pcsselected).getBlockName());
	}
	if (player::inventorybox[si][sj] != 0 && sf == 1) {
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glprint((int)mx, (int)my - 16, BlockInfo(player::inventorybox[si][sj]).getBlockName());
	}
	mousebl = mouseb;
}