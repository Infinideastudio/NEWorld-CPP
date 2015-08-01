#pragma once
#include "Mo.h"

class Cat :public Mo{
private:
	string name = "Cat";
	int type = MoType::ANIMAL;

	//����
	int health = 50;
	int healthMax = 50;
	int healingAbility = 1;

	//�ƶ�
	bool moveable = true;
	bool canFly = false;
	bool canSwim = true;
	double moveSpeed = 1;
	double XPos, YPos, ZPos;

	//ս��
	int fightType = 0; //0 ��ƽ 1 ���� 2 ���� 3 BOSS
	int defense = 0;

	//״̬
	bool inFight = false;

	//Ƥ��
	Skin skin;

public:
	Cat(double x, double y, double z) :XPos(x), YPos(y), ZPos(z), skin(x, y, z){
		skin.loadSkin(SkinTexture::CAT);
	}

	virtual void update();
	virtual void renderer(){ skin.renderer(); }
};