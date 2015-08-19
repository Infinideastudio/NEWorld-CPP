#pragma once
#include "Def.h"
//�ɶ�Ķ�������~�����ˣ�

namespace renderer{
	const int ArrayUNITSIZE = 65536;

	void Init();

	void Vertex3d(double x, double y, double z);

	void TexCoord2d(double x, double y);

	void Color3d(double r, double g, double b);

	void Flush();
}
