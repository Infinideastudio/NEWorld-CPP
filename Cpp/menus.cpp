#include "menus.h"
#include "World.h"
#include "Textures.h"
#include "glprinting.h"
#include <io.h>
#include <fstream>

extern bool gamebegin;

template<typename T>
string strWithVar(string str, T var){
	std::stringstream ss;
	ss << str << var;
	return ss.str();
}

void mainmenu(){
	//主菜单
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 200;
	int midp = windowwidth / 2;
	int rightp = windowwidth / 2 + 200;
	int upp = 280;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_CULL_FACE);
	bool f = false;
	MainForm.Init();
	auto startbtn = MainForm.createbutton("开始游戏");
	auto optionsbtn = MainForm.createbutton(">> 选项...");
	auto quitbtn = MainForm.createbutton("退出");
	do{
		leftp = windowwidth / 2 - 200;
		midp = windowwidth / 2;
		rightp = windowwidth / 2 + 200;
		startbtn->resize(leftp, rightp, upp, upp + 32);
		optionsbtn->resize(leftp, midp - 3, upp + 38, upp + 72);
		quitbtn->resize(midp + 3, rightp, upp + 38, upp + 72);
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		if (startbtn->clicked) worldmenu();
		if (gamebegin) f = true;
		if (optionsbtn->clicked) options();
		if (quitbtn->clicked) exit(0);
		MainForm.render();
		glBindTexture(GL_TEXTURE_2D, guiImage[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(midp - 256, 20);
		glTexCoord2f(0.0f, 0.5f), glVertex2i(midp - 256, 276);
		glTexCoord2f(1.0f, 0.5f), glVertex2i(midp + 256, 276);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(midp + 256, 20);
		glEnd();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwWindowShouldClose(win)) exit(0);
	} while (!f);
	MainForm.cleanup();
}

void options(){
	//设置菜单
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 250;
	int rightp = windowwidth / 2 + 250;
	int midp = windowwidth / 2;
	int upp = 60;
	int downp = windowheight - 20;
	int lspc = 36;
	bool f = false;
	MainForm.Init();
	gui::label*		title = MainForm.createlabel("=================<  选 项  >=================");
	gui::trackbar*  FOVyBar = MainForm.createtrackbar(strWithVar("视野角度：", FOVyNormal), 120, (int)FOVyNormal - 1);
	gui::trackbar*  mmsBar = MainForm.createtrackbar(strWithVar("鼠标灵敏度,", mousemove), 120, (int)mousemove * 40 * 2 - 1);
	gui::trackbar*  viewdistBar = MainForm.createtrackbar(strWithVar("渲染距离,", viewdistance), 120, (viewdistance - 1) * 8 - 1);
	//gui::button*	ciArrayBtn = MainForm.createbutton("使用区块索引数组：" + boolstr(UseCIArray))
	gui::button*	LoadingBtn = MainForm.createbutton("使用加载界面：" + boolstr(useLoading));
	gui::button*	rdstbtn = MainForm.createbutton(">> 渲染选项...");
	gui::button*	gistbtn = MainForm.createbutton(">> 图形界面选项...");
	gui::button*	backbtn = MainForm.createbutton("<< 返回主菜单");
	//gui::button*	savebtn = MainForm.createbutton("保存设置")
	//ciArrayBtn->enabled = false
	//savebtn->enabled = false
	do{
		leftp = windowwidth / 2 - 250;
		rightp = windowwidth / 2 + 250;
		midp = windowwidth / 2;
		downp = windowheight - 20;
		title->resize(midp - 225, midp + 225, 20, 36);
		FOVyBar->resize(leftp, midp - 10, upp + lspc * 0, upp + lspc * 0 + 24);
		mmsBar->resize(midp + 10, rightp, upp + lspc * 0, upp + lspc * 0 + 24);
		viewdistBar->resize(leftp, midp - 10, upp + lspc * 1, upp + lspc * 1 + 24);
		//ciArrayBtn->resize(midp + 10, rightp, upp + lspc * 1, upp + lspc * 1 + 24)
		LoadingBtn->resize(midp + 10, rightp, upp + lspc * 1, upp + lspc * 1 + 24);
		rdstbtn->resize(leftp, midp - 10, upp + lspc * 4, upp + lspc * 4 + 24);
		gistbtn->resize(midp + 10, rightp, upp + lspc * 4, upp + lspc * 4 + 24);
		backbtn->resize(leftp, midp - 10, downp - 24, downp);
		//savebtn->resize(midp + 10, rightp, downp - 24, downp)
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		//更新GUI
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		FOVyNormal = static_cast<float>(FOVyBar->barpos + 1);
		mousemove = (mmsBar->barpos / 2 + 1) / 40.0f;
		viewdistance = viewdistBar->barpos / 8 + 2;
		FOVyBar->text = strWithVar("视野角度：", FOVyNormal);
		mmsBar->text = strWithVar("鼠标灵敏度：", mousemove);
		viewdistBar->text = strWithVar("渲染距离：", viewdistance);
		//ciArrayBtn->text = strWithVar("使用区块索引数组, ", boolstr(UseCIArray))
		LoadingBtn->text = "使用加载界面：" + boolstr(useLoading);
		//if (ciArrayBtn->clicked) UseCIArray = !UseCIArray
		if (LoadingBtn->clicked) useLoading = !useLoading;
		if (rdstbtn->clicked) Renderoptions();
		if (gistbtn->clicked) GUIoptions();
		if (backbtn->clicked) f = true;
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwWindowShouldClose(win)) exit(0);
	} while (!f);
	MainForm.cleanup();
}

void Renderoptions(){
    //渲染设置菜单
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 250;
	int rightp = windowwidth / 2 + 250;
	int midp = windowwidth / 2;
	int downp = windowheight - 20;
	bool f = false;
	MainForm.Init();
	gui::label*  title = MainForm.createlabel("==============<  渲 染 选 项  >==============");
	gui::button*  backbtn = MainForm.createbutton("<< 返回选项菜单");
	do{
		leftp = windowwidth / 2 - 250;
		rightp = windowwidth / 2 + 250;
		midp = windowwidth / 2;
		downp = windowheight - 20;
		title->resize(midp - 225, midp + 225, 20, 36);
		backbtn->resize(leftp, rightp, downp - 24, downp);
		//更新GUI
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		if (backbtn->clicked) f = true;
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwWindowShouldClose(win)) exit(0);
	} while (!f);
    MainForm.cleanup();
}

void GUIoptions(){
    //GUI设置菜单
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 250;
	int rightp = windowwidth / 2 + 250;
	int midp = windowwidth / 2;
	int upp = 60;
	int downp = windowheight - 20;
	int lspc = 36;
	bool f = false;
	MainForm.Init();
	gui::label* title = MainForm.createlabel("===============< 图形界面选项 >==============");
	gui::button* fontbtn = MainForm.createbutton("全部使用Unicode字体：" + boolstr(glprt::useUnicodeASCIIFont));
	gui::button* backbtn = MainForm.createbutton("<< 返回选项菜单");
	do{
        leftp=windowwidth/2-250;
        rightp=windowwidth/2+250;
        midp=windowwidth/2;
        downp=windowheight-20;
        title->resize(midp-225,midp+225,20,36);
        fontbtn->resize(leftp,midp-10,upp+lspc*0,upp+lspc*0+24);
        backbtn->resize(leftp,rightp,downp-24,downp);
        //更新GUI
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
        MainForm.update();
        if(fontbtn->clicked) glprt::useUnicodeASCIIFont=!glprt::useUnicodeASCIIFont;
        if(backbtn->clicked) f=true;
        fontbtn->text="全部使用Unicode字体：" + boolstr(glprt::useUnicodeASCIIFont);
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwWindowShouldClose(win)) exit(0);
	} while (!f);
    MainForm.cleanup();
}

void worldmenu(){
	//世界选择菜单
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 200;
	int midp = windowwidth / 2;
	int rightp = windowwidth / 2 + 200;
	int downp = windowheight - 20;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_CULL_FACE);
	bool refresh = true;
	int selected = 0, mouseon;
	ubyte mblast = 1;
	int  mwlast = 0;
	string chosenWorldName;
	vector<string> worldnames;
	vector<TextureID> thumbnails, texSizeX, texSizeY;
	int trs = 0;
	MainForm.Init();
	auto  title = MainForm.createlabel("==============<  选 择 世 界  >==============");
	auto  vscroll = MainForm.createvscroll(100, 0);
	auto  enterbtn = MainForm.createbutton("进入选定的世界");
	auto  deletebtn = MainForm.createbutton("删除选定的世界");
	auto  backbtn = MainForm.createbutton("<< 返回主菜单");
	world::worldname = "";
	enterbtn->enabled = false;
	deletebtn->enabled = false;
	vscroll->defaultv = true;
	do{
		unsigned int worldcount = worldnames.size();
		leftp = windowwidth / 2 - 250;
		midp = windowwidth / 2;
		rightp = windowwidth / 2 + 250;
		downp = windowheight - 20;
		title->resize(midp - 225, midp + 225, 20, 36);
		vscroll->resize(midp + 275, midp + 295, 36, downp);
		enterbtn->resize(leftp, midp - 10, downp - 60, downp - 36);
		deletebtn->resize(midp + 10, rightp, downp - 60, downp - 36);
		backbtn->resize(leftp, rightp, downp - 24, downp);
		vscroll->barheight = (downp - 72 - 48) / (64 * worldcount + 64)*(downp - 36 - 40);
		if (vscroll->barheight > downp - 36 - 40) vscroll->barheight = downp - 36 - 40;
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		trs = vscroll->barpos / (downp - 36 - 40)*(64 * worldcount + 64);
		mouseon = -1;
		if (mx >= midp - 250 && mx <= midp + 250 && my >= 48 && my <= downp - 72){
			for (unsigned int i = 0; i != worldcount; i++){
				if (my >= 48 + i * 64 - trs&&my <= 48 + i * 64 + 60 - trs){
					if (mb == 1 && mblast == 0){
						chosenWorldName = worldnames[i];
						selected = i;
					}
					mouseon = i;
				}
			}
			if (my >= 48 + worldcount * 64 - trs&&my <= 48 + worldcount * 64 + 60 - trs){
				if (mb == 0 && mblast == 1){
					createworldmenu();
					refresh = true;
				}
				mouseon = worldcount;
			}
		}
		if (enterbtn->clicked){
			gamebegin = true;
			world::worldname = chosenWorldName;
			break;
		}
		if (deletebtn->clicked){
			//删除世界文件
			system((string("rd /s/q Worlds\\") + chosenWorldName + "\\").c_str());
			deletebtn->clicked = false;
			refresh = true;
		}
		if (refresh){
			worldnames.clear();
			thumbnails.clear();
			texSizeX.clear();
			texSizeY.clear();
			worldcount = 0;
			selected = -1;
			mouseon = -1;
			vscroll->barpos = 0;
			chosenWorldName = "";
			//查找所有世界存档
			textures::TEXTURE_RGB tmb;
			long hFile = 0;
			_finddata_t fileinfo;
			if ((hFile = _findfirst(string("Worlds\\*").c_str(), &fileinfo)) != -1)
			{
				do
				{
					if ((fileinfo.attrib &  _A_SUBDIR))
					{
						if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0){
							worldnames.push_back(fileinfo.name);
							std::fstream file;
							file.open("Worlds\\" + string(fileinfo.name) + "\\Thumbnail.bmp", std::ios::in);
							thumbnails.push_back(0);
							texSizeX.push_back(0);
							texSizeY.push_back(0);
							if (file.is_open()){
								tmb = textures::LoadRGBImage("Worlds\\" + string(fileinfo.name) + "\\Thumbnail.bmp");
								glGenTextures(1, &thumbnails[thumbnails.size() - 1]);
								glBindTexture(GL_TEXTURE_2D, thumbnails[thumbnails.size() - 1]);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmb.sizeX, tmb.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tmb.buffer.get());
								texSizeX[texSizeX.size() - 1] = tmb.sizeX;
								texSizeY[texSizeY.size() - 1] = tmb.sizeY;
							}
							file.close();
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);
				_findclose(hFile);
			}

			refresh = false;
		}

		enterbtn->enabled = chosenWorldName != "";
		deletebtn->enabled = chosenWorldName != "";

		if (backbtn->clicked) break;
		MainForm.render();
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, windowheight - (downp - 72), windowwidth, downp - 72 - 48 + 1);
		glTranslatef(0.0f, (float)-trs, 0.0f);
		for (unsigned int i = 0; i != worldcount; i++){
			int xmin, xmax, ymin, ymax;
			xmin = midp - 250, xmax = midp + 250;
			ymin = 48 + i * 64, ymax = 48 + i * 64 + 60;
			if (thumbnails[i] == -1){
				glDisable(GL_TEXTURE_2D);
				if (mouseon == (int)i)
					glColor4f(0.5, 0.5, 0.5, gui::FgA);
				else
					glColor4f(gui::FgR, gui::FgG, gui::FgB, gui::FgA);
				glBegin(GL_QUADS);
				glVertex2i(midp - 250, 48 + i * 64);
				glVertex2i(midp + 250, 48 + i * 64);
				glVertex2i(midp + 250, 48 + i * 64 + 60);
				glVertex2i(midp - 250, 48 + i * 64 + 60);
				glEnd();
			}
			else{
				bool marginOnSides;
				double w, h;
				//计算材质坐标，保持高宽比（按钮大小为500x60），有小学数学基础的人仔细想一想应该能懂QAQ
				if (texSizeX[i] * 60 / 500 < texSizeY[i]){
					marginOnSides = true;
					w = 1.0, h = texSizeX[i] * 60 / 500.0 / texSizeY[i];
				}
				else{
					marginOnSides = false;
					w = texSizeY[i] * 500 / 60.0 / texSizeX[i];
					h = 1.0;
				}
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, thumbnails[i]);
				if (mouseon == (int)i)
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				else
					glColor4f(0.8f, 0.8f, 0.8f, 0.8f);
				glBegin(GL_QUADS);
				glTexCoord2f(static_cast<GLfloat>(0.5 - w / 2), static_cast<GLfloat>(0.5 + h / 2)), glVertex2i(midp - 250, 48 + i * 64);
				glTexCoord2f(static_cast<GLfloat>(0.5 + w / 2), static_cast<GLfloat>(0.5 + h / 2)), glVertex2i(midp + 250, 48 + i * 64);
				glTexCoord2f(static_cast<GLfloat>(0.5 + w / 2), static_cast<GLfloat>(0.5 - h / 2)), glVertex2i(midp + 250, 48 + i * 64 + 60);
				glTexCoord2f(static_cast<GLfloat>(0.5 - w / 2), static_cast<GLfloat>(0.5 - h / 2)), glVertex2i(midp - 250, 48 + i * 64 + 60);
				glEnd();
			}
			glColor4f(gui::FgR*0.9f, gui::FgG*0.9f, gui::FgB*0.9f, 0.9f);
			glDisable(GL_TEXTURE_2D);
			glLineWidth(1.0);
			glBegin(GL_LINE_LOOP);
			glVertex2i(xmin, ymin);
			glVertex2i(xmin, ymax);
			glVertex2i(xmax, ymax);
			glVertex2i(xmax, ymin);
			glEnd();
			if (selected == (int)i){
				glLineWidth(2.0);
				glColor4f(0.0, 0.0, 0.0, 1.0);
				glBegin(GL_LINE_LOOP);
				glVertex2i(midp - 250, 48 + i * 64);
				glVertex2i(midp + 250, 48 + i * 64);
				glVertex2i(midp + 250, 48 + i * 64 + 60);
				glVertex2i(midp - 250, 48 + i * 64 + 60);
				glEnd();
			}
			renderChar((windowwidth - getStrWidth(worldnames[i])) / 2, (140 + i * 128) / 2 - trs, worldnames[i]);
		}
		int i = worldcount;
		glDisable(GL_TEXTURE_2D);
		if (mouseon == i) glColor4f(0.5f, 0.5f, 0.5f, gui::FgA); else glColor4f(gui::FgR, gui::FgG, gui::FgB, gui::FgA);
		glBegin(GL_QUADS);
		glVertex2i(midp - 250, 48 + i * 64);
		glVertex2i(midp + 250, 48 + i * 64);
		glVertex2i(midp + 250, 48 + i * 64 + 60);
		glVertex2i(midp - 250, 48 + i * 64 + 60);
		glEnd();
		glColor4f(gui::FgR*0.9f, gui::FgG*0.9f, gui::FgB*0.9f, 0.9f);
		glDisable(GL_TEXTURE_2D);
		glLineWidth(1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(midp - 250, 48 + i * 64);
		glVertex2i(midp + 250, 48 + i * 64);
		glVertex2i(midp + 250, 48 + i * 64 + 60);
		glVertex2i(midp - 250, 48 + i * 64 + 60);
		glEnd();
		renderChar(static_cast<int>((windowwidth - getStrWidth(">>创建新的世界")) / 2.0), static_cast<int>((140 + i * 128) / 2.0) - trs, ">>创建新的世界");
		glDisable(GL_SCISSOR_TEST);
		glTranslatef(0.0, (float)trs, 0.0);
		mblast = (ubyte)mb;
		mwlast = mw;
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS&&!ESCP || glfwWindowShouldClose(win)) exit(0);
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE) ESCP = false;
	} while (!gamebegin);
	MainForm.cleanup();
}

void createworldmenu(){
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 250;
	int rightp = windowwidth / 2 + 250;
	int midp = windowwidth / 2;
	int downp = windowheight - 20;

	bool worldnametbChanged = false;
	bool f = false;
	MainForm.Init();
	inputstr = "";
	gui::label* title = MainForm.createlabel("==============<  新 建 世 界  >==============");
	gui::textbox* worldnametb = MainForm.createtextbox("输入世界名称");
	gui::button* okbtn = MainForm.createbutton("确定");
	gui::button* backbtn = MainForm.createbutton("<< 返回世界菜单");
	okbtn->enabled = false;
	do{
		leftp = windowwidth / 2 - 250;
		rightp = windowwidth / 2 + 250;
		midp = windowwidth / 2;
		downp = windowheight - 20;
		title->resize(midp - 225, midp + 225, 20, 36);
		worldnametb->resize(midp - 250, midp + 250, 48, 72);
		okbtn->resize(midp - 250, midp + 250, 84, 120);
		backbtn->resize(leftp, rightp, downp - 24, downp);
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		//更新GUI
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		if (worldnametb->pressed && !worldnametbChanged){
			worldnametb->text = "";
			worldnametbChanged = true;
		}
		if (worldnametb->text == "" || !worldnametbChanged||worldnametb->text.find(" ")!=-1) okbtn->enabled = false; else okbtn->enabled = true;
		if (okbtn->clicked || backbtn->clicked) f = true;
		inputstr = "";
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) exit(0);
	} while (!f);
	if (worldnametbChanged){
		world::worldname = worldnametb->text;
		gamebegin = true;
	}
	MainForm.cleanup();
}
