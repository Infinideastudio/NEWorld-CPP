#pragma once
#include "Def.h"
#include "World.h"

namespace world {
	class chunkBuildManage {
	public:
		chunkBuildManage(int x, int z) : cx(x), cz(z) {
			id = getChunkID(x, 0, z);
		};
		int cx, cz;
		uint64 id;
		int terrain[16][16];
		int sandTerrain[16][16];

		void build();
	};
}