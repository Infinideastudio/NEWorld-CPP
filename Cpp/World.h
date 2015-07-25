#pragma once
#include "Def.h"
#include "ChunkIndexArray.h"
#include "Chunk.h"

namespace world{
	extern string worldname;
	extern int worldsize;
	extern int worldheight;
	extern brightness skylight;         //Sky light level;
	extern brightness BRIGHTNESSMAX;     //Maximum brightness;
	extern brightness BRIGHTNESSMIN;    //Mimimum brightness;
	extern brightness BRIGHTNESSDEC;    //Brightness decree;
	extern uint EmptyList;

	extern chunk* chunks;
	extern int loadedChunks;
	extern int ciCacheIndex;
	extern uint64 ciCacheID;
	extern chunkIndexArray ciArray;
	extern bool ciArrayAval;

	extern sbyte cloud[128][128];
	extern int rebuiltChunks, rebuiltChunksCount;
	extern int updatedChunks, updatedChunksCount;
	extern int unloadedChunks, unloadedChunksCount;
	extern int chunkRenderList[65536][4]; //1 to
	extern int chunkLoadList[65536][4]; //1 to
	extern int chunkUnloadList[65536][4]; //1 to
	extern int chunkRenders, chunkLoads, chunkUnloads;

	void Init();

	void AddChunk(int x, int y, int z);
	void DeleteChunk(int x, int y, int z);
	uint64 getChunkID(int x, int y, int z);
	bool chunkLoaded(int x, int y, int z);
	bool chunkMatched(chunk c, int x, int y, int z);
	int getChunkIndex(int x, int y, int z);
	chunk* getChunkPtr(int x, int y, int z, bool silentMode=false);
	void ExpandChunkArray(int cc);
	void ReduceChunkArray(int cc);

	int getchunkpos(int n);
	int getblockpos(int n);

	vector<Hitbox::AABB> getHitboxes(Hitbox::AABB box);
	bool inWater(Hitbox::AABB box);

	void renderblock(int x, int y, int z);
	void updateblock(int x, int y, int z, bool blockchanged);
	block getblock(int x, int y, int z, block defaultRet = blocks::AIR);
	brightness getbrightness(int x, int y, int z);
	void setblock(int x, int y, int z, block Blockname);
	void setbrightness(int x, int y, int z, brightness ibrightness);
	void putblock(int x, int y, int z, block Blockname);
	void pickblock(int x, int y, int z);

	bool chunkOutOfBound(int x, int y, int z);
	bool chunkInRange(int x, int y, int z, int px, int py, int pz, int dist);
	bool chunkUpdated(int x, int y, int z);
	void setChunkUpdated(int x, int y, int z, bool value);
	void sortChunkRenderList(int xpos, int ypos, int zpos);
	void sortChunkLoadList(int xpos, int ypos, int zpos);
	void sortChunkUnloadList(int xpos, int ypos, int zpos);
	void qsortList(int List[][4], int first, int last, bool unloadsort);

	void saveAllChunks();
	void destroyAllChunks();
	void saveGame();
	void loadGame();

	void buildtree(int x, int y, int z);
}
