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

	//生命
	int health;
	int healthMax;
	int healingAbility;

	//移动
	bool moveable = true;
	bool canFly;
	bool canSwim;
	int moveSpeed = 10;
	
	//战斗
	int fightType; //0 和平 1 中立 2 主动 3 BOSS
	int attack;
	int defense;
	int field;
	bool ignoreWallView;

	//状态
	bool inFight = false;

	//皮肤
	Skin skin;

	//掉落
	bool dropEquipment;
	int dropEquipmentChance;
	vector<std::pair<int, ItemPair>> dropThings;  //掉落概率-掉落物

	//装备
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