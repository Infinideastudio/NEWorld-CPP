#pragma once
#include "Def.h"
#include "ChunkIndexArray.h"
#include "ChunkHeightMap.h"
#include "Chunk.h"
#include "Mo.h"

namespace world{
	class chunkHeightMap;
	extern string worldname;
	const int worldsize = 134217728;
	const int worldheight = 8;
	extern brightness skylight;         //Sky light level
	extern brightness BRIGHTNESSMAX;     //Maximum brightness
	extern brightness BRIGHTNESSMIN;    //Mimimum brightness
	extern brightness BRIGHTNESSDEC;    //Brightness decree
	extern uint EmptyList;

	extern chunk* chunks;
	extern int loadedChunks;
	extern int allocChunks;
	extern int ciCacheIndex;
	extern uint64 ciCacheID;
	extern chunkIndexArray ciArray;
	extern bool ciArrayAval;
	extern set<chunkHeightMap> CHMs;

	extern bool cloud[128][128];
	extern int rebuiltChunks, rebuiltChunksCount;
	extern int updatedChunks, updatedChunksCount;
	extern int unloadedChunks, unloadedChunksCount;
	//extern pair<int, chunk*> chunkRenderList[65536];
	//extern int chunkLoadList[65536][4];
	//extern int chunkUnloadList[65536][4];
	extern int chunkBuildRenderList[256][2];
	extern int chunkLoadList[256][4];
	extern int chunkUnloadList[256][4];
	extern vector<uint*> displayListUnloadList;
	extern int chunkBuildRenders, chunkLoads, chunkUnloads;

	extern vector<shared_ptr<Mo>> MOs;

	void Init();

	chunk* AddChunk(int x, int y, int z);
	void DeleteChunk(int ci);
	uint64 getChunkID(int x, int y, int z);
	bool chunkLoaded(int x, int y, int z);
	bool chunkMatched(chunk c, int x, int y, int z);
	int getChunkIndex(int x, int y, int z);
	chunk* getChunkPtr(int x, int y, int z, bool silentMode=false);
	void ExpandChunkArray(int cc);
	void ReduceChunkArray(int cc);

	//给出N对应的chunk坐标，XYZ通用
	inline int getchunkpos(int n) {
		return n >> 4;
	}

	//给出N对应的chunk内的方块坐标，XYZ通用
	inline int getblockpos(int n) {
		return n & 0xf;
	}

	vector<Hitbox::AABB> getHitboxes(Hitbox::AABB box);
	bool inWater(Hitbox::AABB box);

	void renderblock(int x, int y, int z, chunk* chunkptr);
	void updateblock(int x, int y, int z, bool blockchanged);
	block getblock(int x, int y, int z, block defaultRet = blocks::AIR, chunk* defaultchunkptr = nullptr);
	brightness getbrightness(int x, int y, int z);
	void setblock(int x, int y, int z, block Blockname);
	void setbrightness(int x, int y, int z, brightness ibrightness);
	void putblock(int x, int y, int z, block Blockname);
	void pickblock(int x, int y, int z);

	bool chunkOutOfBound(int x, int y, int z);
	bool chunkInRange(int x, int y, int z, int px, int py, int pz, int dist);
	bool chunkUpdated(int x, int y, int z);
	void setChunkUpdated(int x, int y, int z, bool value);
	//void sortChunkRenderList(int xpos, int ypos, int zpos);
	//void sortChunkLoadList(int xpos, int ypos, int zpos);
	//void sortChunkUnloadList(int xpos, int ypos, int zpos);
	//void qsortList(int List[][4], int first, int last, bool unloadsort);
	//void qsortListPtr(pair<int, chunk*> List[], int first, int last, bool unloadsort);
	void sortChunkBuildRenderList(int xpos, int ypos, int zpos);
	void sortChunkLoadUnloadList(int xpos, int ypos, int zpos);

	void saveAllChunks();
	void destroyAllChunks();

	void buildtree(int x, int y, int z);
}
