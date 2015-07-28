#include "glprinting.h"
#include "Textures.h"
#include <windows.h>

namespace glprt{
	uint gbe;
	uint Font;
	int gloop;
	int ww, wh;
	float r, g, b, a;
	uint unicodeTex[256];
	bool unicodeTexAval[256];
	bool useUnicodeASCIIFont;
}

void BuildFont(int w, int h){
	glprt::ww = w;
	glprt::wh = h;
	glprt::Font = textures::LoadFontTexture("textures\\Fonts\\ascii.bmp");

	float cx, cy;

	glprt::gbe = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, glprt::Font);
	for (glprt::gloop = 0; glprt::gloop != 255; glprt::gloop++){

		cx = (glprt::gloop % 16) / 16.0f;
		cy = (int(glprt::gloop / 16)) / 16.0f;

		glNewList(glprt::gbe + glprt::gloop, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(cx, 1 - cy);
		glVertex2i(0, 0);
		glTexCoord2f(cx + 0.0625f, 1.0f - cy);
		glVertex2i(16, 0);
		glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.0625f);
		glVertex2i(16, 16);
		glTexCoord2f(cx, 1.0f - cy - 0.0625f);
		glVertex2i(0, 16);
		glEnd();
		glTranslated(10.0, 0.0, 0.0);
		glEndList();

	}
}

void setFontColor(float r, float g, float b, float a){
	glprt::r = r;
	glprt::g = g;
	glprt::b = b;
	glprt::a = a;
}

bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if (dwSize < dwMinSize) return false;

	MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);
	return true;
}

int getStrWidth(string s){
	int ret = 0;
	int i = 0;
	LPWSTR wstr = new WCHAR[128];
	memset(wstr, 0, sizeof(wstr));
	MByteToWChar(s.c_str(), wstr, 128 / sizeof(wstr[0]));
	for (int k = 0; k < lstrlenW(wstr); k++){
		if (s[i] >= 0 && s[i] <= 127){
			i += 1;
			ret += 10;
		}
		else{
			i += 2;
			ret += 16;
		}
	}
	delete[] wstr;
	return ret;
}

void glprint(int x, int y, string glstring){
	glBindTexture(GL_TEXTURE_2D, glprt::Font);
	//glDisable GL_DEPTH_TEST;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glprt::ww, glprt::wh, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glColor4f(0.5, 0.5, 0.5, glprt::a);
	glTranslated(x + 1, y + 1, 0);
	glListBase(glprt::gbe);
	glCallLists(glstring.length(), GL_UNSIGNED_BYTE, glstring.c_str());

	glLoadIdentity();
	glColor4f(glprt::r, glprt::g, glprt::b, glprt::a);
	glTranslated(x, y, 0);
	glListBase(glprt::gbe);
	glCallLists(glstring.length(), GL_UNSIGNED_BYTE, glstring.c_str());

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	//glEnable GL_DEPTH_TEST;
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

void renderChar(int x, int y, string glstring){
	
	int i = 0, k, uc;
	double tx, ty, span = 0;
	textures::TEXTURE_RGBA Tex;
	uint ftex;

	LPWSTR wstr = new WCHAR[128];
	memset(wstr, 0, sizeof(wstr));
	MByteToWChar(glstring.c_str(), wstr, 128 / sizeof(wstr[0]));

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glprt::ww, glprt::wh, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	for (k = 0; k < lstrlenW(wstr); k++){
		glLoadIdentity();
		glColor4f(glprt::r, glprt::g, glprt::b, glprt::a);
		glTranslated(x + 1 + span, y + 1, 0);
		uc = wstr[k];
		if (!glprt::useUnicodeASCIIFont && glstring[i] >= 0 && glstring[i] <= 127){
			glprint(x + 1 + (int)span, y + 1, string() + glstring[i]);
		}
		else{
			if (!glprt::unicodeTexAval[uc / 256]) {
				/*color 15:*/printf("[Console][Event]");
				/*color 7 :*/printf("Loading unicode font texture #%d\n", uc / 256);
				std::stringstream ss;
				ss << "Textures\\Fonts\\unicode\\unicode_glyph_" << uc / 256 << ".bmp";
				ftex = textures::LoadFontTexture(ss.str());
				glprt::unicodeTex[uc / 256] = ftex;
				glprt::unicodeTexAval[uc / 256] = true;
			}
			else{
				ftex = glprt::unicodeTex[uc / 256];
			}

			tx = ((uc % 256) % 16) / 16.0;
			ty = 1 - ((uc % 256) / 16) / 16.0;
			//glScaled(2, 2, 2);
			glBindTexture(GL_TEXTURE_2D, ftex);
			glBegin(GL_QUADS);
			glColor4f(0.5, 0.5, 0.5, glprt::a);
			glTexCoord2d(tx, ty);
			glVertex2i(1, 1);
			glTexCoord2d(tx + 0.0625, ty);
			glVertex2i(17, 1);
			glTexCoord2d(tx + 0.0625, ty - 0.0625);
			glVertex2i(17, 17);
			glTexCoord2d(tx, ty - 0.0625);
			glVertex2i(1, 17);
			glColor4f(glprt::r, glprt::g, glprt::b, glprt::a);
			glTexCoord2d(tx, ty);
			glVertex2i(0, 0);
			glTexCoord2d(tx + 0.0625, ty);
			glVertex2i(16, 0);
			glTexCoord2d(tx + 0.0625, ty - 0.0625);
			glVertex2i(16, 16);
			glTexCoord2d(tx, ty - 0.0625);
			glVertex2i(0, 16);
			glEnd();
		}
		if (glstring[i] >= 0 && glstring[i] <= 127){
			//k += 1;
			i += 1;
			span += 10;//*2;
		}
		else{
			i += 2;
			span += 16;//*2; //10
		}
	}

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glColor4f(1.0, 1.0, 1.0, 1.0);
	delete[] wstr;
}