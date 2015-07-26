#include "menus.h"
#include "World.h"
#include "Textures.h"
#include "glprinting.h"

extern int windowwidth;
extern int windowheight;
extern double mx;
extern double my;
extern int mw;
extern int mb;
extern bool gamebegin;
extern bool ESCP;
extern float FOVyNormal;
extern float mousemove;
extern int viewdistance;
extern bool UseCIArray;
extern string inputstr;

template<typename T>
string strWithVar(string str, T var){
	std::stringstream ss;
	ss << str << var;
	return ss.str();
}

void mainmenu(){
	//主菜单;
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
	auto faststartbtn = MainForm.createbutton("快速开始游戏");
	auto label1 = MainForm.createlabel("开发者:qiaozhanrong[小桥],abc612008[Null],Jelawat地鼠,renfei147...");
	auto label2 = MainForm.createlabel("向Minecraft制作团队表达敬意！");
	do{
		leftp = windowwidth / 2 - 200;
		midp = windowwidth / 2;
		rightp = windowwidth / 2 + 200;
		faststartbtn->resize(leftp, rightp, upp, upp + 32);
		startbtn->resize(leftp, rightp, upp + 72 + 6, upp + 72 + 6 + (72 - 38));
		optionsbtn->resize(leftp, midp - 3, upp + 38, upp + 72);
		quitbtn->resize(midp + 3, rightp, upp + 38, upp + 72);
		label1->resize(0, windowwidth, windowheight - 32, windowheight - 16);
		label2->resize(0, windowwidth, windowheight - 16, windowheight);
		glfwGetCursorPos(win, &mx, &my);
		//mw=glfwGetMouseWheel();
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		if (startbtn->clicked) worldmenu();
		if (gamebegin) f = true;
		if (optionsbtn->clicked) options();
		if (quitbtn->clicked) exit(0);
		if (faststartbtn->clicked) gamebegin = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, guiImage[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(0, 0);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(windowwidth, 0);
		glTexCoord2f(1.0f, 0.45f), glVertex2i(windowwidth, windowheight);
		glTexCoord2f(0.0f, 0.45f), glVertex2i(0, windowheight);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, guiImage[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(midp - 256, 20);
		glTexCoord2f(0.0f, 0.5f), glVertex2i(midp - 256, 276);
		glTexCoord2f(1.0f, 0.5f), glVertex2i(midp + 256, 276);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(midp + 256, 20);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS&&ESCP == false || glfwWindowShouldClose(win)) exit(0);
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE) ESCP = false;
	} while (!f);
	MainForm.cleanup();
}

void options(){
	//设置菜单;
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
	gui::button*	ciArrayBtn = MainForm.createbutton("使用区块索引数组：" + boolstr(UseCIArray));
	gui::button*	rdstbtn = MainForm.createbutton(">> 渲染选项...");
	gui::button*	gistbtn = MainForm.createbutton(">> 图形界面选项...");
	gui::button*	backbtn = MainForm.createbutton("<< 返回主菜单");
	//gui::button*	savebtn = MainForm.createbutton("保存设置");
	//ciArrayBtn->enabled = false;
	//savebtn->enabled = false;
	do{
		leftp = windowwidth / 2 - 250;
		rightp = windowwidth / 2 + 250;
		midp = windowwidth / 2;
		downp = windowheight - 20;
		title->resize(midp - 225, midp + 225, 20, 36);
		FOVyBar->resize(leftp, midp - 10, upp + lspc * 0, upp + lspc * 0 + 24);
		mmsBar->resize(midp + 10, rightp, upp + lspc * 0, upp + lspc * 0 + 24);
		viewdistBar->resize(leftp, midp - 10, upp + lspc * 1, upp + lspc * 1 + 24);
		ciArrayBtn->resize(midp + 10, rightp, upp + lspc * 1, upp + lspc * 1 + 24);
		rdstbtn->resize(leftp, midp - 10, upp + lspc * 4, upp + lspc * 4 + 24);
		gistbtn->resize(midp + 10, rightp, upp + lspc * 4, upp + lspc * 4 + 24);
		backbtn->resize(leftp, midp - 10, downp - 24, downp);
		//savebtn->resize(midp + 10, rightp, downp - 24, downp);
		glfwGetCursorPos(win, &mx, &my);
		//SIGN mw=glfwGetMouseWheel();
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		//更新GUI;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		FOVyNormal = static_cast<float>(FOVyBar->barpos + 1);
		mousemove = (mmsBar->barpos / 2 + 1) / 40.0f;
		viewdistance = viewdistBar->barpos / 8 + 2;
		FOVyBar->text = strWithVar("视野角度：", FOVyNormal);
		mmsBar->text = strWithVar("鼠标灵敏度：", mousemove);
		viewdistBar->text = strWithVar("渲染距离：", viewdistance);
		ciArrayBtn->text = strWithVar("使用区块索引数组, ", boolstr(UseCIArray));
		if (ciArrayBtn->clicked) UseCIArray = !UseCIArray;
		//if (rdstbtn->clicked) renderoptions();
		//if (gistbtn->clicked) GUIoptions();
		if (backbtn->clicked) f = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, guiImage[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(0, 0);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(windowwidth, 0);
		glTexCoord2f(1.0f, 0.45f), glVertex2i(windowwidth, windowheight);
		glTexCoord2f(0.0f, 0.45f), glVertex2i(0, windowheight);
		glEnd();
		//渲染控件;
		glDisable(GL_TEXTURE_2D);
		MainForm.render();
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) exit(0);
	} while (!f);
	MainForm.cleanup();
}
/* 偷懒
void Renderoptions(){
    //渲染设置菜单;
     MainForm gui.Form;
     leftp int = windowwidth/2-250;
     rightp int = windowwidth/2+250;
     midp int = windowwidth/2;
     upp int = 60;
     downp int = windowheight-20;
     lspc int = 36;
     bool f;
    MainForm.Init();
     title gui.label    ptr = MainForm.CreateLabel("==============<  渲 染 选 项  >==============");
     backbtn gui.button ptr = MainForm.CreateButton("<< 返回选项菜单");
    do;
        leftp=windowwidth/2-250;
        rightp=windowwidth/2+250;
        midp=windowwidth/2;
        downp=windowheight-20;
        title->resize(midp-225,midp+225,20,36);
        backbtn->resize(leftp,rightp,downp-24,downp);
        glfwGetMousePos(@mx,@my);
        mw=glfwGetMouseWheel();
        //更新GUI;
        MainForm.mousedata(mx,my,mw,mb);
        MainForm.update();
        if( backbtn->clicked=true then f=true;
        glClear(GL_COLOR_BUFFER_BIT||GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glortho(0,windowwidth,windowheight,0,-1.0,1.0);
        glDepthFunc(GL_ALWAYS);
        glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glcolor4f(1.0,1.0,1.0,1.0);
        glbindtexture(GL_TEXTURE_2D,guiImage(1));
        glbegin(GL_QUADS);
            gltexcoord2f(0.0,1.0),glvertex2i(0,0);
            gltexcoord2f(1.0,1.0),glvertex2i(windowwidth,0);
            gltexcoord2f(1.0,0.45),glvertex2i(windowwidth,windowheight);
            gltexcoord2f(0.0,0.45),glvertex2i(0,windowheight);
        glend();
        //渲染控件;
        glDisable(GL_TEXTURE_2D);
        MainForm.render();
        glfwSwapBuffers();
        if( glfwGetKey(GLFW_KEY_ESC)=GLFW_PRESS||glfwGetWindowParam(GLFW_OPENED)=0 then end;
    loop until f=true;
    MainForm.cleanup();
}
void GUIoptions(){
    //GUI设置菜单;
     MainForm gui.Form;
     leftp int = windowwidth/2-250;
     rightp int = windowwidth/2+250;
     midp int = windowwidth/2;
     upp int = 60;
     downp int = windowheight-20;
     lspc int = 36;
     bool f;
    MainForm.Init();
     title gui.label    ptr = MainForm.CreateLabel("===============< 图形界面选项 >==============");
     fontbtn gui.button ptr = MainForm.CreateButton("全部使用Unicode字体：" & boolstr(glprt.useUnicodeASCIIFont));
     backbtn gui.button ptr = MainForm.CreateButton("<< 返回选项菜单");
    do;
        leftp=windowwidth/2-250;
        rightp=windowwidth/2+250;
        midp=windowwidth/2;
        downp=windowheight-20;
        title->resize(midp-225,midp+225,20,36);
        fontbtn->resize(leftp,midp-10,upp+lspc*0,upp+lspc*0+24);
        backbtn->resize(leftp,rightp,downp-24,downp);
        glfwGetMousePos(@mx,@my);
        mw=glfwGetMouseWheel();
        //更新GUI;
        MainForm.mousedata(mx,my,mw,mb);
        MainForm.update();
        if( fontbtn->clicked=true then glprt.useUnicodeASCIIFont=true-glprt.useUnicodeASCIIFont;
        if( backbtn->clicked=true then f=true;
        fontbtn->text="全部使用Unicode字体：" & boolstr(glprt.useUnicodeASCIIFont);
        glClear(GL_COLOR_BUFFER_BIT||GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glortho(0,windowwidth,windowheight,0,-1.0,1.0);
        glDepthFunc(GL_ALWAYS);
        glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glcolor4f(1.0,1.0,1.0,1.0);
        glbindtexture(GL_TEXTURE_2D,guiImage(1));
        glbegin(GL_QUADS);
            gltexcoord2f(0.0,1.0),glvertex2i(0,0);
            gltexcoord2f(1.0,1.0),glvertex2i(windowwidth,0);
            gltexcoord2f(1.0,0.45),glvertex2i(windowwidth,windowheight);
            gltexcoord2f(0.0,0.45),glvertex2i(0,windowheight);
        glend();
        //渲染控件;
        glDisable(GL_TEXTURE_2D);
        MainForm.render();
        glfwSwapBuffers();
        if( glfwGetKey(GLFW_KEY_ESC)=GLFW_PRESS||glfwGetWindowParam(GLFW_OPENED)=0 then end;
    loop until f=true;
    MainForm.cleanup();
}
*/
void worldmenu(){
	//世界选择菜单;
	int i;
	gui::Form MainForm;
	int leftp = windowwidth / 2 - 200;
	int midp = windowwidth / 2;
	int rightp = windowwidth / 2 + 200;
	int downp = windowheight - 20;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_CULL_FACE);
	bool f = false, refresh = true;
	int worldcount = 0;
	int selected = 0, mouseon;
	string  directoryname;
	ubyte mblast = 1;
	int  mwlast = 0;
	vector<string> worldnames;
	vector<uint> thumbnails, texSizeX, texSizeY;
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
		glfwGetCursorPos(win, &mx, &my);
		//mw=glfwGetMouseWheel();
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		trs = vscroll->barpos / (downp - 36 - 40)*(64 * worldcount + 64);
		mouseon = -1;
		if (mx >= midp - 250 && mx <= midp + 250 && my >= 48 && my <= downp - 72){
			for (i = 0; i != worldcount; i++){
				if (my >= 48 + i * 64 - trs&&my <= 48 + i * 64 + 60 - trs){
					if (mb == 1 && mblast == 0){
						world::worldname = worldnames[i];
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
				mouseon = i;
			}
		}
		if (enterbtn->clicked){
			//if (world::worldname == "client") mpclient = true; else mpclient = false;
			//if (world::worldname == "server") mpserver = true; else mpserver = false;
			gamebegin = true;
			f = true;
		}
		if (deletebtn->clicked){
			//删除世界文件;
			//偷懒
			//directoryname = dir(exepath & "\\Worlds\\" & world::worldname & "\\chunks\\*.*", FileAttribArchive);
			//do{
			//	if (len(directoryname) > 0 && directoryname!="."&&directoryname!=".."){
			//		kill(exepath & "\\Worlds\\" & world.worldname & "\\chunks\\" & directoryname);
			//	}
			//	directoryname = dir();
			//} while (len(directoryname) > 0);
			//rmdir(exepath & "\\Worlds\\" & world.worldname & "\\chunks");
			//kill(exepath & "\\Worlds\\" & world.worldname & "\\Thumbnail.bmp");
			//kill(exepath & "\\Worlds\\" & world.worldname & "\\player.NEWorldPlayer");
			//rmdir(exepath & "\\Worlds\\" & world.worldname);
			deletebtn->clicked = false;
			refresh = true;
		}
		//if (refresh){
		//	textures::TEXTURE_RGB tmb; //Thumbnail buffer
		//	worldcount = 0;
		//	selected = -1;
		//	mouseon = -1;
		//	world::worldname = "";
		//	//计算存档数量;
		//	directoryname = dir("Worlds\\*", FileAttribDirectory);
		//	do{
		//		if (len(directoryname) > 0 && directoryname!="."&&directoryname!="..") worldcount += 1;
		//		directoryname = dir();
		//	} while (directoryname.length() > 0);
		//	worldnames.push_back("");
		//	thumbnails.push_back(0);
		//	texSizeX.push_back(0);
		//	texSizeY.push_back(0);
		//	vscroll->barpos = 0;
		//	//查找所有世界存档;
		//	directoryname = dir("Worlds\\*", FileAttribDirectory);
		//	i = 0;
		//	do{
		//		if (len(directoryname) > 0 && directoryname!="."&&directoryname!=".."){
		//			worldnames[i] = directoryname;
		//			if (fileExists("Worlds\\" & directoryname & "\\Thumbnail.bmp")){
		//				//printf( "[Console][Event]Loading thumbnail of world " & chr(34) & directoryname & chr(34);
		//				tmb = textures::LoadRGBTexture("Worlds\\" & directoryname & "\\Thumbnail.bmp");
		//				glGenTextures(1, &thumbnails[i]);
		//				glBindTexture(GL_TEXTURE_2D, thumbnails[i]);
		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmb.sizeX, tmb.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tmb.buffer);
		//				texSizeX[i] = tmb.sizeX;
		//				texSizeY[i] = tmb.sizeY;
		//				free(tmb.buffer);
		//			}
		//			else{
		//				thumbnails[i] = -1;
		//			}
		//			i += 1;
		//		}
		//		directoryname = dir();
		//	} while (len(directoryname) > 0);
		//	refresh = false;
		//}

		enterbtn->enabled = world::worldname != "";
		deletebtn->enabled = world::worldname != "";

		if (backbtn->clicked) f = true;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, guiImage[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(0, 0);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(windowwidth, 0);
		glTexCoord2f(1.0f, 0.45f), glVertex2i(windowwidth, windowheight);
		glTexCoord2f(0.0f, 0.45f), glVertex2i(0, windowheight);
		glEnd();
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, windowheight - (downp - 72), windowwidth, downp - 72 - 48 + 1);
		glTranslatef(0.0f, (float)-trs, 0.0f);
		for (i = 0; i != worldcount; i++){
			int xmin, xmax, ymin, ymax;
			xmin = midp - 250, xmax = midp + 250;
			ymin = 48 + i * 64, ymax = 48 + i * 64 + 60;
			if (thumbnails[i] == -1){
				glDisable(GL_TEXTURE_2D);
				if (mouseon == i)
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
				//计算材质坐标，保持高宽比（按钮大小为500x60），有小学数学基础的人仔细想一想应该能懂QAQ;
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
				if (mouseon == i)
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
			if (selected == i){
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
		MainForm.render();
		mblast = (ubyte)mb;
		mwlast = mw;
		glfwSwapBuffers(win);
		glfwPollEvents();
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS&&!ESCP || glfwWindowShouldClose(win)) exit(0);
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE) ESCP = false;
	} while (!f);
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
	gui::textbox*  worldnametb = MainForm.createtextbox("输入世界名称");
	gui::button*  okbtn = MainForm.createbutton("确定");
	gui::button*  backbtn = MainForm.createbutton("<< 返回世界菜单");
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
		glfwGetCursorPos(win, &mx, &my);
		//SIGN mw=glfwGetMouseWheel();
		mb = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
		//更新GUI;
		MainForm.mousedata((int)mx, (int)my, mw, mb);
		MainForm.update();
		if (worldnametb->pressed && !worldnametbChanged){
			worldnametb->text = "";
			worldnametbChanged = true;
		}
		if (worldnametb->text == "" || !worldnametbChanged) okbtn->enabled = false; else okbtn->enabled = true;
		if (okbtn->clicked == true){
			//mkdir(exepath & "\\Worlds");
			//mkdir(exepath & "\\Worlds\\" & worldnametb->text);
			//mkdir(exepath & "\\Worlds\\" & worldnametb->text & "\\chunks");
			f = true;
		}
		inputstr = "";
		if (backbtn->clicked) f = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowwidth, windowheight, 0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, guiImage[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f), glVertex2i(0, 0);
		glTexCoord2f(1.0f, 1.0f), glVertex2i(windowwidth, 0);
		glTexCoord2f(1.0f, 0.45f), glVertex2i(windowwidth, windowheight);
		glTexCoord2f(0.0f, 0.45f), glVertex2i(0, windowheight);
		glEnd();
		//渲染控件;
		glDisable(GL_TEXTURE_2D);
		MainForm.render();
		glfwSwapBuffers(win);
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) exit(0);
	} while (!f);
	MainForm.cleanup();
}
