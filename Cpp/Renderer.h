#pragma once
#include "Def.h"
//可恶的顶点数组~我来了！

namespace renderer{
	const int ArrayUNITSIZE = 65536;

	void Init();

	void Vertex3d(double x, double y, double z);

	void TexCoord2d(double x, double y);

	void Color3d(double r, double g, double b);

	void Vertex3d(float x, float y, float z);

	void TexCoord2d(float x, float y);

	void Color3d(float r, float g, float b);

	void Flush(uint& buffer, int& vtxs);

	void renderbuffer(uint buffer, int vtxs, bool ftex, bool fcol);
}
