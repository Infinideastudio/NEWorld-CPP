#include "Cat.h"

void Cat::update(){
	static int way = 0; //方向
	if (rnd() < 0.3)way = rand() % 4;  //有概率改变方向
	if (rnd() < 0.3) return;
	switch (way){
	case 0:
		XPos += moveSpeed;
		break;
	case 1:
		YPos += moveSpeed;
		break;
	case 2:
		XPos -= moveSpeed;
		break;
	case 3:
		YPos -= moveSpeed;
		break;
	}
}