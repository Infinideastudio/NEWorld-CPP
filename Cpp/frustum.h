#include "Def.h"
#include "Hitbox.h"

namespace Frustum{
	extern double m_Frustum[24];
	extern float proj[16], modl[16];
	extern double clip[16];

	void normalize(int side);
	void calc();
	bool cubeInFrustum(double x1, double y1, double z1, double x2, double y2, double z2);
	bool aabbInFrustum(Hitbox::AABB aabb);
}

