#pragma once
#include "Def.h"
#include "Hitbox.h"

namespace particles{
	const int PARTICALE_MAX = 4096;
	struct particle{
		bool exist = false;
		double xpos, ypos, zpos;
		float xsp, ysp, zsp, psize, tcX, tcY;
		int lasts;
		Hitbox::AABB hb;
	};
	extern vector<particle> ptcs;
	extern int ptcsrendered;

	void update(particle &ptc);

	void updateall();

	void render(particle &ptc);

	void renderall();

	void throwParticle(block pt, float x, float y, float z, float xs, float ys, float zs, float psz, int last);

}