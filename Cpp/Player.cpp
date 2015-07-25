#include "Player.h"
#include "World.h"
#include <fstream>
#include <sstream>
bool FLY;      //·ÉÐÐ;
bool CROSS;    //´©Ç½ ¡û_¡û (Superman!);
extern float playerHeight;
extern float playerHeightExt;
//extern const uint32 VERSION;
extern ubyte itemInHand;
extern block inventorybox[4][10];
extern block inventorypcs[4][10];
namespace player{
	Hitbox::AABB playerbox;
	//int livesoverall = 100;
	//int lives = 100;
	//int livestarget = 100;

	double xa, ya, za;
	bool OnGround = false;
	bool Running = false;
	bool NearWall = false;
	bool inWater = false;
	float speed;
	int AirJumps;
	int cxt, cyt, czt, cxtl, cytl, cztl;
	double lookupdown, heading, xpos, ypos, zpos, xposold, yposold, zposold, jump;
	float xlookspeed, ylookspeed;
	int intxpos, intypos, intzpos, intxposold, intyposold, intzposold;
	double renderxpos, renderypos, renderzpos;

	void InitHitbox(){
		playerbox.xmin = -0.3;
		playerbox.xmax = 0.3;
		playerbox.ymin = -0.8;
		playerbox.ymax = 0.8;
		playerbox.zmin = -0.3;
		playerbox.zmax = 0.3;
	}

	void InitPosition(){
		xposold = xpos;
		yposold = ypos;
		zposold = zpos;
		cxt = world::getchunkpos((int)xpos); cxtl = cxt;
		cyt = world::getchunkpos((int)ypos); cytl = cyt;
		czt = world::getchunkpos((int)zpos); cztl = czt;
	}

	void MoveHitbox(double x, double y, double z){
		Hitbox::MoveTo(playerbox, x, y + 0.5, z);
	}

	void MoveHitboxToPosition(){
		MoveHitbox(xpos, ypos, zpos);
	}

	void Move(){
		inWater = world::inWater(playerbox);
		if (!FLY && !CROSS && inWater){
			xa *= 0.6;
			ya *= 0.6;
			za *= 0.6;
		}

		double xal = xa;
		double yal = ya;
		double zal = za;

		if (!CROSS){
			
			auto Hitboxes = world::getHitboxes(Hitbox::Expand(playerbox, xa, ya, za));
			int num = Hitboxes.size();
			if (num > 0){
				for (int i = 0; i < num; i++){
					ya = Hitbox::MaxMoveOnYclip(playerbox, Hitboxes[i], ya);
				}
				Hitbox::Move(playerbox, 0.0, ya, 0.0);
				for (int i = 0; i < num; i++){
					xa = Hitbox::MaxMoveOnXclip(playerbox, Hitboxes[i], xa);
				}
				Hitbox::Move(playerbox, xa, 0.0, 0.0);
				for (int i = 0; i < num; i++){
					za = Hitbox::MaxMoveOnZclip(playerbox, Hitboxes[i], za);
				}
				Hitbox::Move(playerbox, 0.0, 0.0, za);
			}
			if (ya != yal && yal<0.0) OnGround = true; else OnGround = false;
			if (ya != yal && yal>0.0) jump = 0.0;
			if (xa != xal || za != zal) NearWall = true; else NearWall = false;
			xpos += xa;
			ypos += ya;
			zpos += za;
			xa *= 0.8;
			za *= 0.8;
			if (OnGround){

				//°ë¶×ÌØÐ§;
				if (player::jump < -0.005){
					//player::jump /= 2;
					if (player::jump <= -(playerHeight - 0.5f))
						playerHeightExt = -(playerHeight - 0.5f);
					else
						playerHeightExt = (float)player::jump;
				}
				else{
					if (playerHeightExt <= -0.05f) playerHeightExt *= 0.8f;
				}

				xa *= 0.7;
				za *= 0.7;
			}
			if (FLY)  ya *= 0.8;
		}
		else{
			xpos += xa;
			ypos += ya;
			zpos += za;

			xa *= 0.8;
			ya *= 0.8;
			za *= 0.8;
		}

		cxtl = cxt; cytl = cyt; cztl = czt;
		cxt = (int)world::getchunkpos((int)xpos);
		cyt = (int)world::getchunkpos((int)ypos);
		czt = (int)world::getchunkpos((int)zpos);

		MoveHitboxToPosition();
	}

	bool putblock(int x, int y, int z, block blockname) {
		Hitbox::AABB blockbox;
		bool success = false;
		blockbox.xmin = x - 0.5;
		blockbox.xmax = x + 0.5;
		blockbox.ymin = y - 0.5;
		blockbox.ymax = y + 0.5;
		blockbox.zmin = z - 0.5;
		blockbox.zmax = z + 0.5;
		if (((Hitbox::Hit(playerbox, blockbox) == false) || CROSS || BlockInfo(blockname).isSolid() == false) && BlockInfo(world::getblock(x, y, z)).isSolid() == false){
			world::putblock(x, y, z, blockname);
			success = true;
		}
		return success;
	}

	void save(string worldn){
		uint32 curversion = VERSION;

		std::stringstream ss;
		ss << "Worlds\\" << worldn << "player.NEWorldPlayer";
		std::ofstream isave(ss.str(), std::ios::binary | std::ios::out);
		if (!isave.is_open()) return;
		isave << curversion << OnGround << Running << AirJumps << lookupdown << heading << xpos << ypos << zpos
			  << jump << xlookspeed << ylookspeed << FLY << CROSS;
		isave.write((char*)inventorybox, sizeof(inventorybox));
		isave.write((char*)inventorypcs, sizeof(inventorypcs));
		isave << itemInHand;
		isave.close();
	}

	void load(string worldn){
		uint32 targetVersion;
		std::stringstream ss;
		ss <<"Worlds\\" << worldn << "player.NEWorldPlayer";
		std::ifstream iload(ss.str(), std::ios::binary | std::ios::in);
		if (!iload.is_open()) return;
		iload >> targetVersion;
		if (targetVersion != VERSION) return;
		iload >> OnGround >> Running >> AirJumps >> lookupdown >> heading
			  >> xpos >> ypos >> zpos >> jump >> xlookspeed >> ylookspeed >> FLY >> CROSS;
		iload.read((char*)inventorybox, sizeof(inventorybox));
		iload.read((char*)inventorypcs, sizeof(inventorypcs));
		iload.close();
	}

}
