#pragma once
#include "Def.h"
#include "Hitbox.h"

namespace world{
	class chunk{
	private:
		bool Modified = false;
		block pblocks[16][16][16];
		brightness pbrightness[16][16][16];
		bool updated = false;

	public:
		int cx, cy, cz;
		uint64 id;
		uint vbuffer[3];
		int vertexes[3];
		bool isBuilt = false;
		float loadAnim = 0;
		bool isEmptyChunk = false;
		bool isHiddenChunk = false;

		void Init(int cxi, int cyi, int czi, uint64 idi);
		void create();
		void destroy();
		void Load();
		void Unload();
		void build();
		string getFileName();
		bool fileExist();
		void LoadFromFile();
		void SaveToFile();
		void buildlists();
		void destroylists();

		inline block getblock(ubyte x, ubyte y, ubyte z) {
			//获取区块内的方块
			if (isEmptyChunk) return blocks::AIR;
			return pblocks[x][y][z];
		}
		brightness getbrightness(ubyte x, ubyte y, ubyte z);
		inline void setblock(ubyte x, ubyte y, ubyte z, block iblock) {
			//设置方块
			if (this == nullptr) return;
			pblocks[x][y][z] = iblock;
			Modified = true;
			if (iblock != blocks::AIR) isEmptyChunk = false;
		}
		void setbrightness(ubyte x, ubyte y, ubyte z, brightness tbrightness);
		void putblock(ubyte x, ubyte y, ubyte z, block iblock);
		void pickblock(ubyte x, ubyte y, ubyte z);

		inline block getblock(int x, int y, int z) {
			return getblock((ubyte)x, (ubyte)y, (ubyte)z);
		}
		brightness getbrightness(int x, int y, int z);
		inline void setblock(int x, int y, int z, block iblock) {
			setblock((ubyte)x, (ubyte)y, (ubyte)z, iblock);
		}
		void setbrightness(int x, int y, int z, brightness tbrightness);

		void putblock(int x, int y, int z, block iblock);
		void pickblock(int x, int y, int z);

		bool getUpdated();
		void setUpdated(bool value);
		void callList(int l);

		Hitbox::AABB chunk::getChunkAABB();
		Hitbox::AABB getRelativeAABB(double x, double y, double z);
		void hiddenChunkTest();
	};
}