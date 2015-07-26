#pragma once
#include "Def.h"

enum SkinType{ NORMALSKIN, CATSKIN };

class Skin{
private:
	double X, Y, Z;
	GLuint texture = -1;
	int skinType;
public:
	Skin(){};
	Skin(double x, double y, double z) :X(x), Y(y), Z(z){}
	void renderer();
	void loadSkin(string path, string pathMask, int type = NORMALSKIN);
};