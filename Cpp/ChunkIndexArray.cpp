#include "ChunkIndexArray.h"
namespace world{
	void chunkIndexArray::setsize(int s){
		size = s;
		size2 = size*size;
		size3 = size*size*size;
	}

	bool chunkIndexArray::create(){
		int x, y, z;
		Array = (int*)malloc(size3*sizeof(int));
		if (Array == nullptr) return true;
		for (x = 0; x != size; x++){
			for (y = 0; y != size; y++){
				for (z = 0; z != size; z++){
					Array[x*size2 + y*size + z] = -1;
				}
			}
		}
		return false;
	}

	void chunkIndexArray::destroy(){
		free(Array);
		Array = 0;
	}

	void chunkIndexArray::move(int xd, int yd, int zd){
		if (Array == nullptr) return;
		chunkIndexArray arrTemp;
		int x, y, z;
		arrTemp.setsize(size);
		arrTemp.create();
		for (x = 0; x != size; x++){
			for (y = 0; y != size; y++){
				for (z = 0; z != size; z++){
					if (elementExists(x + xd, y + yd, z + zd))
						arrTemp.Array[x*size2 + y*size + z] = Array[(x + xd)*size2 + (y + yd)*size + (z + zd)];
					else
						arrTemp.Array[x*size2 + y*size + z] = -1;
				}
			}
		}
		destroy();
		Array = arrTemp.Array;
	}

	void chunkIndexArray::moveTo(int x, int y, int z){
		if (Array == nullptr) return;
		move(x - originX, y - originY, z - originZ);
	}

	void chunkIndexArray::AddChunk(int ci){
		if (Array == nullptr) return;
		int x, y, z;
		for (x = 0; x != size; x++){
			for (y = 0; y != size; y++){
				for (z = 0; z != size; z++){
					if (Array[x*size2 + y*size + z] >= ci) Array[x*size2 + y*size + z]++;
				}
			}
		}
	}

	void chunkIndexArray::DeleteChunk(int ci){
		if (Array == nullptr) return;
		int x, y, z;
		for (x = 0; x != size; x++){
			for (y = 0; y != size; y++){
				for (z = 0; z != size; z++){
					if (Array[x*size2 + y*size + z] == ci) Array[x*size2 + y*size + z] = -1;
					if (Array[x*size2 + y*size + z] > ci) Array[x*size2 + y*size + z]--;
				}
			}
		}
	}

	bool chunkIndexArray::elementExists(int x, int y, int z){
		if (x < 0) return false;
		if (x >= size) return false;
		if (y < 0) return false;
		if (y >= size) return false;
		if (z < 0) return false;
		if (z >= size) return false;
		return true;
	}

	bool chunkIndexArray::chunkIndexExists(int x, int y, int z){
		if (Array == nullptr) return false;
		if (!elementExists(x - originX, y - originY, z - originZ)) return false;
		if (Array[(x - originX)*size2 + (y - originY)*size + (z - originZ)] < 0) return false;
		return true;
	}

	int chunkIndexArray::getChunkIndex(int x, int y, int z){
		if (Array == nullptr) return -1;
		return Array[(x - originX)*size2 + (y - originY)*size + (z - originZ)];
	}
}