#include "Cat.h"

void Cat::update(){
	static int way = 0; //����
	if (rnd() < 0.3)way = rand() % 4;  //�и��ʸı䷽��
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