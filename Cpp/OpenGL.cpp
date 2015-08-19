#include "OpenGL.h"
#include "Player.h"
#include "glprinting.h"

//OpenGL
int GLVersionMajor, GLVersionMinor, GLVersionRev;

void CurPosCall(GLFWwindow*, double x, double y) {
	mx = x / (retina ? 2 : 1);
	my = y / (retina ? 2 : 1);
}

void CharInputFun(GLFWwindow *, unsigned int c) {
	if (c >= 128) {
		wchar_t* pwszUnicode = new wchar_t[2];
		pwszUnicode[0] = (wchar_t)c;
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
	else {
		inputstr += (char)c;
	}
}

void winsizecall(GLFWwindow*, int width, int height) {
	windowwidth = width;
	windowheight = height > 0 ? height : 1;
	setupscreen();
}

void scrollEvent(GLFWwindow*, double, double offsety) {
	//mw = offsety
	player::itemInHand -= (ubyte)sgn(static_cast<int>(offsety));
	if (player::itemInHand > 9) player::itemInHand = 0;
	if (player::itemInHand < 0) player::itemInHand = 9;
}

void CenterScreen() {
	//   int w,h
	//GLFWvidmode v
	// w=v.width
	// h=v.height
	// w-=windowwidth
	// h-=windowheight
	// glfwSetWindowPos(w/2,h/2)
}

void FullScreen() {
	//GLFWvidmode v
	//   glfwGetDesktopMode(&v)
	//   windowwidth=v.width
	//   windowheight=v.height
	//   glfwCloseWindow()
	//   glfwOpenWindow(windowwidth,windowheight,0,0,0,0,32,0,GLFW_FULLSCREEN)
	//   setupScreen()
}

void setupscreen() {

	//OpenGL参数设置
	glViewport(0, 0, windowwidth*(retina ? 2 : 1), windowheight*(retina ? 2 : 1));
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
	glAlphaFunc(GL_GREATER, 0.0); //<--这家伙在卖萌？(往后面看看，卖萌的多着呢)
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

void InitGL() {
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

void setupNormalFog() {

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

void setupDepthFog(int d) {

	glEnable(GL_FOG);
	uint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
	float fogColor[4] = { skycolorR, skycolorG, skycolorB, 1.0f };
	uint fogfilter = 2;

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, &fogColor[0]);
	glFogf(GL_FOG_START, float(d - 8));
	glFogf(GL_FOG_END, float(d + 8));

}

void setupWaterFog() {

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
