#pragma once
#include "Def.h"

enum MoType{
	NORMAL,
	ANIMAL,
	SKELETON,
	SPIDER
};

class Mo{
private:
	string name;
	int type;

	//����
	int health;
	int healthMax;
	int healingAbility;

	//�ƶ�
	bool moveable = true;
	bool canFly;
	bool canSwim;
	int moveSpeed = 10;
	
	//ս��
	int fightType; //0 ��ƽ 1 ���� 2 ���� 3 BOSS
	int attack;
	int defense;
	int field;
	bool ignoreWallView;

	//״̬
	bool inFight = false;

	//Ƥ��
	Skin skin;

	//����
	bool dropEquipment;
	int dropEquipmentChance;
	vector<std::pair<int, ItemPair>> dropThings;  //�������-������

	//װ��
	int equipment[4];

public:
	virtual inline string getName() const { return name; }
	virtual inline int getType() const { return type; }
	virtual inline int getHealth() const { return health; }
	virtual inline int getMaxHealth() const { return healthMax; }

	virtual void init();
	virtual void update();
	virtual void renderer(){
		skin.renderer();
	}

};