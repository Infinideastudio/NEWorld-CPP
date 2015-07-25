#pragma once
#include "Def.h"

namespace world{
	struct chunkIndexArray{

		int* Array;
		int originX, originY, originZ, size, size2, size3;

		void setsize(int s);
		bool create();
		void destroy();
		void move(int xd, int yd, int zd);
		void moveTo(int x, int y, int z);
		void AddChunk(int ci);
		void DeleteChunk(int ci);
		bool elementExists(int x, int y, int z);
		bool chunkIndexExists(int x, int y, int z);
		int getChunkIndex(int x, int y, int z);

	};
}
