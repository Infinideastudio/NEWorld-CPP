#include "Particles.h"
#include "World.h"
#include "Textures.h"
#include "Renderer.h"
#include "Player.h"
#include "frustum.h"

namespace particles{
	vector<particle> ptcs;
	int ptcsrendered;

	void update(particle &ptc){

		if (ptc.lasts < 30)
			ptc.psize *= 0.9f;

		double dx, dy, dz;
		float psz = ptc.psize;

		ptc.hb.xmin = ptc.xpos - psz;
		ptc.hb.xmax = ptc.xpos + psz;
		ptc.hb.ymin = ptc.ypos - psz;
		ptc.hb.ymax = ptc.ypos + psz;
		ptc.hb.zmin = ptc.zpos - psz;
		ptc.hb.zmax = ptc.zpos + psz;

		dx = ptc.xsp;
		dy = ptc.ysp;
		dz = ptc.zsp;

		auto Hitboxes = world::getHitboxes(Hitbox::Expand(ptc.hb, dx, dy, dz));
		int hitnum = Hitboxes.size();
		for (int i = 0; i < hitnum; i++){
			dy = Hitbox::MaxMoveOnYclip(ptc.hb, Hitboxes[i], dy);
		}
		Hitbox::Move(ptc.hb, 0.0, dy, 0.0);
		for (int i = 0; i < hitnum; i++){
			dx = Hitbox::MaxMoveOnXclip(ptc.hb, Hitboxes[i], dx);
		}
		Hitbox::Move(ptc.hb, dx, 0.0, 0.0);
		for (int i = 0; i < hitnum; i++){
			dz = Hitbox::MaxMoveOnZclip(ptc.hb, Hitboxes[i], dz);
		}
		Hitbox::Move(ptc.hb, 0.0, 0.0, dz);

		ptc.xpos += dx;
		ptc.ypos += dy;
		ptc.zpos += dz;
		if (dy != ptc.ysp) ptc.ysp = 0.0;
		ptc.xsp *= 0.6f;
		ptc.zsp *= 0.6f;
		ptc.ysp -= 0.01f;
		ptc.lasts -= 1;
	}

	void updateall(){
		for (auto iter = ptcs.begin(); iter< ptcs.end();){
			if (!iter->exist) continue;
			update(*iter);
			if (iter->lasts <= 0){
				iter->exist = false;
				iter = ptcs.erase(iter);
			}
			else{
				iter++;
			}
		}
	}

	void render(particle &ptc){
		//if (!Frustum::aabbInFrustum(ptc.hb)) return
		ptcsrendered++;
		float size = BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE*ptc.psize;
		float col = world::getbrightness((int)ptc.xpos, (int)ptc.ypos, (int)ptc.zpos) / (float)world::BRIGHTNESSMAX;
		float col1 = col * 0.5f;
		float col2 = col * 0.7f;
		float tcx = ptc.tcX;
		float tcy = ptc.tcY;
		float psize = ptc.psize;
		double xpos = ptc.xpos - player::xpos;
		double ypos = ptc.ypos - player::ypos - player::height - player::heightExt;
		double zpos = ptc.zpos - player::zpos;

		glColor3d(col1, col1, col1);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos - psize, zpos + psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos - psize, zpos + psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos + psize, zpos + psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos + psize, zpos + psize);

		glColor3d(col1, col1, col1);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos + psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos + psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos - psize, zpos - psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos - psize, zpos - psize);

		glColor3d(col, col, col);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos + psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos + psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos + psize, zpos + psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos + psize, zpos + psize);

		glColor3d(col, col, col);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos - psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos - psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos - psize, zpos + psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos - psize, zpos + psize);

		glColor3d(col2, col2, col2);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos + psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos + psize, ypos + psize, zpos + psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos - psize, zpos + psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos + psize, ypos - psize, zpos - psize);

		glColor3d(col2, col2, col2);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos - psize, zpos - psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 0.0);
		glVertex3d(xpos - psize, ypos - psize, zpos + psize);
		glTexCoord2d(tcx + size * 1.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos + psize, zpos + psize);
		glTexCoord2d(tcx + size * 0.0, tcy + size * 1.0);
		glVertex3d(xpos - psize, ypos + psize, zpos - psize);
	}

	void renderall(){
		glBegin(GL_QUADS);
		ptcsrendered = 0;
		for (unsigned int i = 0; i != ptcs.size(); i++){
			if (!ptcs[i].exist) continue;
			render(ptcs[i]);
		}
		glEnd();
	}

	void throwParticle(block pt, float x, float y, float z, float xs, float ys, float zs, float psz, int last){
		float tcX1 = (float)textures::getTexcoordX(pt, 2);
		float tcY1 = (float)textures::getTexcoordY(pt, 2);
		particle ptc;
		ptc.exist = true;
		ptc.xpos = x;
		ptc.ypos = y;
		ptc.zpos = z;
		ptc.xsp = xs;
		ptc.ysp = ys;
		ptc.zsp = zs;
		ptc.psize = psz;
		ptc.hb.xmin = x - psz;
		ptc.hb.xmax = x + psz;
		ptc.hb.ymin = y - psz;
		ptc.hb.ymax = y + psz;
		ptc.hb.zmin = z - psz;
		ptc.hb.zmax = z + psz;
		ptc.lasts = last;
		ptc.tcX = tcX1 + (float)rnd()*(BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE)*(1.0f - psz);
		ptc.tcY = tcY1 + (float)rnd()*(BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE)*(1.0f - psz);
		ptcs.push_back(ptc);
	}
}
