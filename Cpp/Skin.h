#pragma once
#include "Def.h"

enum SkinType{ NORMAL, CAT };

class Skin{
private:
	double X, Y, Z;
	GLuint texture;
	int skinType;
public:
	Skin(double x, double y, double z) :X(x), Y(y), Z(z){}
	void renderer();
	void loadSkin(string path, string pathMask, int type = NORMAL);
};