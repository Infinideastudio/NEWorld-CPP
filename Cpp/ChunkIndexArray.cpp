#include "ChunkIndexArray.h"
namespace world{
	void chunkIndexArray::setsize(int s){
		size = s;
		size2 = size*size;
		size3 = size*size*size;
	}

	bool chunkIndexArray::create(){
		Array = new int[size*3];
		if (Array == nullptr) return false;
		for (int i = 0; i != size3; i++)
			Array[i] = -1;
		return true;
	}

	void chunkIndexArray::destroy(){
		delete[] Array;
		Array = nullptr;
	}

	void chunkIndexArray::move(int xd, int yd, int zd){
		int* arrTemp = new int[size3];
		for (int x = 0; x != size; x++) {
			for (int y = 0; y != size; y++) {
				for (int z = 0; z != size; z++) {
					if (elementExists(x + xd, y + yd, z + zd))
						arrTemp[x*size2 + y*size + z] = Array[(x + xd)*size2 + (y + yd)*size + (z + zd)];
					else
						arrTemp[x*size2 + y*size + z] = -1;
				}
			}
		}
		delete[] arrTemp;
		Array = arrTemp;
		originX += xd; originY += yd; originZ += zd;
	}

	void chunkIndexArray::moveTo(int x, int y, int z){
		move(x - originX, y - originY, z - originZ);
	}

	void chunkIndexArray::AddChunk(int ci, int cx, int cy, int cz) {
		for (int x = 0; x != size; x++) {
			for (int y = 0; y != size; y++) {
				for (int z = 0; z != size; z++) {
					if (Array[x*size2 + y*size + z] >= ci) Array[x*size2 + y*size + z]++;
				}
			}
		}
		cx -= originX;
		cy -= originY;
		cz -= originZ;
		if (elementExists(cx, cy, cz)) Array[cx*size2 + cy*size + cz] = ci;
	}

	void chunkIndexArray::DeleteChunk(int ci){
		for (int x = 0; x != size; x++) {
			for (int y = 0; y != size; y++) {
				for (int z = 0; z != size; z++) {
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

    int chunkIndexArray::getChunkIndex(int x,int y,int z){
		x -= originX; y -= originY; z -= originZ;
		if (!elementExists(x, y, z))  return -1;
		return Array[x*size2 + y*size + z];
    }
}