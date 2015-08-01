#pragma once
#include "Mo.h"

class Cat :public Mo{
private:
	string name = "Cat";
	int type = MoType::ANIMAL;

	//生命
	int health = 50;
	int healthMax = 50;
	int healingAbility = 1;

	//移动
	bool moveable = true;
	bool canFly = false;
	bool canSwim = true;
	double moveSpeed = 1;
	double XPos, YPos, ZPos;

	//战斗
	int fightType = 0; //0 和平 1 中立 2 主动 3 BOSS
	int defense = 0;

	//状态
	bool inFight = false;

	//皮肤
	Skin skin;

public:
	Cat(double x, double y, double z) :XPos(x), YPos(y), ZPos(z), skin(x, y, z){
		skin.loadSkin(SkinTexture::CAT);
	}

	virtual void update();
	virtual void renderer(){ skin.renderer(); }
};