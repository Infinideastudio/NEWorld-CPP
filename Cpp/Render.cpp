#include "Render.h"
#include "OpenGL.h"
#include "glprinting.h"
#include "Player.h"
#include "frustum.h"
#include "Textures.h"
#include "GUI.h"
#include "Particles.h"
#include "Renderer.h"

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
	if (!bagOpened && !loading) {
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

		glfwSetCursorPos(win, windowwidth / 2 * (retina ? 2 : 1), windowheight / 2 * (retina ? 2 : 1));
	}

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

	if (world::CHMs.size() > 1000) world::CHMs.clear();

	//更新区块显示列表
	//world::sortChunkRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
	//for (int i = 1; i <= world::chunkRenders; i++) {
	//	auto cptr = world::chunkRenderList[i].second;
	//	cptr->buildlists();
	//}
	static bool FirstFrameThisUpdate;
	if (FirstFrameThisUpdate) {
		world::sortChunkBuildRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
		FirstFrameThisUpdate = false;
	}
	int brl = world::chunkBuildRenders > 4 ? 3 : world::chunkBuildRenders - 1;
	for (int i = 0; i <= brl; i++) {
		int ci = world::chunkBuildRenderList[i][1];
		world::chunks[ci].buildlists();
	}
	//删除已卸载区块的显示列表
	for (int i = 0; i != 5; i++) {
		if (world::displayListUnloadList.size() <= 0) break;
		glDeleteBuffersARB(3, world::displayListUnloadList[0]);
		world::displayListUnloadList.pop_back();
	}
	//if ((timer() - uctime) >= 1.0) {
	//	uctime = timer()
	//	ups = upsc
	//	upsc = 0
	//}
	struct RenderChunk
	{
	public:
		RenderChunk(world::chunk* c) {
			cx = c->cx;
			cy = c->cy;
			cz = c->cz;
			vbuffers = c->vbuffer;
			vtxs = c->vertexes;
			loadAnim = c->loadAnim;
		}
		int cx, cy, cz;
		uint* vbuffers = nullptr;
		float loadAnim = 0;
		int* vtxs = nullptr;
	};
	double plookupdown = player::lookupdown;
	double pheading = player::heading;
	glLoadIdentity();
	glRotated(plookupdown, 1, 0, 0);
	glRotated(360.0 - pheading, 0, 1, 0);
	Frustum::calc();
	vector<RenderChunk> displayChunks;
	for (int i = 0; i != world::loadedChunks; i++) {
		if (world::chunks[i].isEmptyChunk) continue;
		if (world::chunks[i].isBuilt && world::chunkInRange(world::chunks[i].cx, world::chunks[i].cy, world::chunks[i].cz, player::cxt, player::cyt, player::czt, viewdistance)) {
			if (Frustum::AABBInFrustum(world::chunks[i].getRelativeAABB(xpos, ypos, zpos))) {
				displayChunks.push_back(&world::chunks[i]);
			}
		}
	}
	MutexUnlock();
	renderedChunk = displayChunks.size();
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		if (cr.vtxs[0] == 0) continue;
		glPushMatrix();
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		renderer::renderbuffer(cr.vbuffers[0], cr.vtxs[0], true, true);
		glPopMatrix();
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (seldes > 0.0) {
		glTranslated(selx - xpos, sely - ypos, selz - zpos);
		renderDestroy(seldes, 0, 0, 0);
		glTranslated(-selx + xpos, -sely + ypos, -selz + zpos);
	}
	glBindTexture(GL_TEXTURE_2D, BlockTextures);
	particles::renderall();
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
	//glBindTexture(GL_TEXTURE_2D, BlockTextures);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		if (cr.vtxs[1] == 0) continue;
		glPushMatrix();
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		renderer::renderbuffer(cr.vbuffers[1], cr.vtxs[1], true, true);
		glPopMatrix();
	}
	glDisable(GL_CULL_FACE);
	for (int i = 0; i != renderedChunk; i++) {
		auto cr = displayChunks[i];
		if (cr.vtxs[2] == 0) continue;
		glPushMatrix();
		glTranslated(cr.cx * 16 - xpos, cr.cy * 16 - cr.loadAnim - ypos, cr.cz * 16 - zpos);
		renderer::renderbuffer(cr.vbuffers[2], cr.vtxs[2], true, true);
		glPopMatrix();
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glLoadIdentity();
	glRotated(player::lookupdown, 1, 0, 0);
	glRotated(360.0 - player::heading, 0, 1, 0);
	glTranslated(-xpos, -ypos, -zpos);
	MutexLock();
	glDisable(GL_TEXTURE_2D);
	drawcloud(player::xpos, player::zpos);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//MutexLock()

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
	if (world::getblock(int(player::xpos + 0.5), int(player::ypos + player::height + player::heightExt + 0.5), int(player::zpos + 0.5)) == blocks::WATER) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		double tcX = textures::getTexcoordX(blocks::WATER, 1);
		double tcY = textures::getTexcoordY(blocks::WATER, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(tcX, tcY + 1 / 8.0); glVertex2i(0, 0);
		glTexCoord2f(tcX, tcY); glVertex2i(0, windowheight);
		glTexCoord2f(tcX + 1 / 8.0, tcY); glVertex2i(windowwidth, windowheight);
		glTexCoord2f(tcX + 1 / 8.0, tcY + 1 / 8.0); glVertex2i(windowwidth, 0);
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
	MutexUnlock();
	glfwSwapBuffers(win);
	glfwPollEvents();
	MutexLock();
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

	TextureID splTex = textures::LoadRGBATexture("textures\\gui\\SplashScreen.bmp", "");

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
		glVertex2d(i*32, windowheight - 32);
		glTexCoord2f(0.0, 0.0);
		glVertex2d(i*32 + 32, windowheight - 32);
		glTexCoord2f(1.0, 0.0);
		glVertex2d(i*32 + 32, windowheight);
		glTexCoord2f(1.0, 1.0);
		glVertex2d(i*32, windowheight);
		glEnd();
		if (player::inventorybox[3][i] != blocks::AIR) {
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, BlockTextures);
			double tcX = textures::getTexcoordX(player::inventorybox[4][i], 1);
			double tcY = textures::getTexcoordY(player::inventorybox[4][i], 1);
			glBegin(GL_QUADS);
			glTexCoord2f(tcX, tcY + 1 / 8.0);
			glVertex2d(i * 32 + 2, windowheight - 32 + 2);
			glTexCoord2f(tcX, tcY);
			glVertex2d(i * 32 + 30, windowheight - 32 + 2);
			glTexCoord2f(tcX + 1 / 8.0, tcY);
			glVertex2d(i * 32 + 30, windowheight - 32 + 30);
			glTexCoord2f(tcX + 1 / 8.0, tcY + 1 / 8);
			glVertex2d(i * 32 + 2, windowheight - 32 + 30);
			glEnd();
			std::stringstream ss;
			ss << (int)player::inventorypcs[3][i];
			glprint(i * 32, windowheight - 32, ss.str());
		}
	}

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
		ss << "ChunkHeightMap Size = " << world::CHMs.size();
		debugText(ss.str()); ss.str("");

		debugText("", true);
	}
	else {

		setFontColor(1.0f, 1.0f, 1.0f, 0.9f);
		std::stringstream ss;
		ss << "v" << VERSION;
		glprint(0, 0, ss.str());
		ss.clear();
		ss.str("");
		ss << "fps " << fps;
		glprint(0,16, ss.str());

	}

	//检测帧速率
	if (timer() - fctime >= 1) {
		fps = fpsc;
		fpsc = 0;
		fctime = timer();
	}
	fpsc++;
}

void drawLoading() {
	MutexLock();
	int loadedChunks = world::loadedChunks;
	world::sortChunkBuildRenderList(RoundInt(player::xpos), RoundInt(player::ypos), RoundInt(player::zpos));
	int brl = world::chunkBuildRenders > 4 ? 3 : world::chunkBuildRenders - 1;
	for (int i = 0; i <= brl; i++) {
		int ci = world::chunkBuildRenderList[i][1];
		world::chunks[ci].buildlists();
	}
	MutexUnlock();
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
		MutexLock();
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
	glFogf(GL_FOG_START, 100.0);
	glFogf(GL_FOG_END, 300.0);
	static double ltimer;
	static bool generated;
	static uint cloudvb[128];
	static int vtxs[128];
	static float f;
	static int l;
	if (ltimer == 0.0) ltimer = timer();
	f += (timer() - ltimer)*0.25;
	ltimer = timer();
	if (f >= 1.0) {
		l += f;
		f -= int(f);
		l %= 128;
	}

	if (!generated) {
		generated = true;
		for (int i = 0; i != 128; i++) {
			for (int j = 0; j != 128; j++) {
				world::cloud[i][j] = int(rnd() * 2);
			}
		}
		glGenBuffersARB(128, &cloudvb[0]);
		for (int i = 0; i != 128; i++) {
			renderer::Init();
			for (int j = 0; j != 128; j++) {
				if (world::cloud[i][j]) {
					renderer::Vertex3d(j*cloudwidth, 128.0, 0.0);
					renderer::Vertex3d(j*cloudwidth, 128.0, cloudwidth);
					renderer::Vertex3d((j + 1)*cloudwidth, 128.0, cloudwidth);
					renderer::Vertex3d((j + 1)*cloudwidth, 128.0, 0.0);
				}
			}
			renderer::Flush(cloudvb[i], vtxs[i]);
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	for (int i = 0; i != 128; i++) {
		glPushMatrix();
		glTranslatef(-64 * cloudwidth, 0, cloudwidth*((l + i) % 128 + f) - 64 * cloudwidth);
		renderer::renderbuffer(cloudvb[i], vtxs[i], false, false);
		glPopMatrix();
	}
	setupNormalFog();
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
				glBindTexture(GL_TEXTURE_2D, BlockTextures);
				double tcX = textures::getTexcoordX(player::inventorybox[i][j], 1);
				double tcY = textures::getTexcoordY(player::inventorybox[i][j], 1);
				glBegin(GL_QUADS);
				glTexCoord2f(tcX, tcY + 1 / 8.0);
				glVertex2d(j*(32 + 8) + 2 + leftp, i*(32 + 8) + 2 + upp);
				glTexCoord2f(tcX, tcY);
				glVertex2d(j*(32 + 8) + 30 + leftp, i*(32 + 8) + 2 + upp);
				glTexCoord2f(tcX + 1 / 8.0, tcY);
				glVertex2d(j*(32 + 8) + 30 + leftp, i*(32 + 8) + 30 + upp);
				glTexCoord2f(tcX + 1 / 8.0, tcY + 1 / 8.0);
				glVertex2d(j*(32 + 8) + 2 + leftp, i*(32 + 8) + 30 + upp);
				glEnd();
				std::stringstream ss;
				ss << (int)player::inventorypcs[i][j];
				glprint(j*(32 + 8) + 8 + leftp, (i*(32 + 16) + 8 + upp), ss.str());
			}
		}
	}
	if (itemselected != blocks::AIR) {
		glBindTexture(GL_TEXTURE_2D, BlockTextures);
		double tcX = textures::getTexcoordX(itemselected, 1);
		double tcY = textures::getTexcoordY(itemselected, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(tcX, tcY + 1 / 8.0);
		glVertex2d(mx - 16, my - 16);
		glTexCoord2f(tcX, tcY);
		glVertex2d(mx + 16, my - 16);
		glTexCoord2f(tcX + 1 / 8.0, tcY);
		glVertex2d(mx + 16, my + 16);
		glTexCoord2f(tcX + 1 / 8.0, tcY + 1 / 8.0);
		glVertex2d(mx - 16, my + 16);
		glEnd();
		std::stringstream ss;
		ss << pcsselected;
		glprint((int)mx + 4, (int)my + 16, ss.str());
	}
	if (player::inventorybox[si][sj] != 0 && sf == 1) {
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glprint((int)mx, (int)my - 16, BlockInfo(player::inventorybox[si][sj]).getBlockName());
	}
	mousebl = mouseb;
}