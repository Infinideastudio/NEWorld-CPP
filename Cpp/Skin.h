#pragma once
#include "Def.h"

enum SkinType{ NORMALSKIN, CATSKIN };

class Skin{
private:
	double X, Y, Z;
	TextureID texture = 0;
	int skinType = 0;
public:
	Skin(){};
	Skin(double x, double y, double z) :X(x), Y(y), Z(z){}
	void renderer();
	void loadSkin(string path, string pathMask, int type = NORMALSKIN);
};