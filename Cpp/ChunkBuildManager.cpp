#include "ChunkBuildManager.h"
#include "WorldGen.h"

void world::chunkBuildManage::build()
{
	for (int x = 0; x != 16; x++) {
		for (int z = 0; z != 16; z++) {
			terrain[x][z] = WorldGen::getHeight(cx * 16 + x, cz * 16 + z);
			sandTerrain[x][z] = WorldGen::getSandHeight(cx * 16 + x, cz * 16 + z);
		}
	}
}
