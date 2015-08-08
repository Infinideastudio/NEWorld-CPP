#include "Def.h"
#include "Hitbox.h"

namespace Frustum{
	void normalize(int side);
	void calc();
	bool cubeInFrustum(double x1, double y1, double z1, double x2, double y2, double z2);
	bool aabbInFrustum(Hitbox::AABB aabb);
}

