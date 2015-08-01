#pragma once
#include "Def.h"

enum SkinTexture{ PLAYER, CAT };
class Skin{
private:
	double X, Y, Z;
	TextureID texture = 0;
	int skin = 0;
public:
	Skin(){};
	Skin(double x, double y, double z) :X(x), Y(y), Z(z){}
	void renderer();
	void loadSkin(int _skin = PLAYER);
};