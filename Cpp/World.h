#pragma once
#include "Def.h"
#include "ChunkIndexArray.h"
#include "ChunkBuildManager.h"
#include "Chunk.h"
#include "Mo.h"

namespace world{
	extern string worldname;
	const int worldsize = 134217728;
	const int worldheight = 128;
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
	extern vector<chunkBuildManage> CBM;

	extern sbyte cloud[256][256];
	extern int rebuiltChunks, rebuiltChunksCount;
	extern int updatedChunks, updatedChunksCount;
	extern int unloadedChunks, unloadedChunksCount;
	extern pair<int, chunk*> chunkRenderList[65536];
	extern int chunkLoadList[65536][4];
	extern int chunkUnloadList[65536][4];
	//extern pair<int, chunk*> chunkUnloadList[65536]
	extern vector<int> displayListUnloadList;
	extern int chunkRenders, chunkLoads, chunkUnloads;

	extern vector<shared_ptr<Mo>> MOs;

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

	//void renderblock(int x, int y, int z)
	void renderblock(int x, int y, int z, int cx, int cy, int cz);
	void renderblock(int x, int y, int z, chunk* chunkptr);
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
	void qsortListPtr(pair<int, chunk*> List[], int first, int last, bool unloadsort);

	void saveAllChunks();
	void destroyAllChunks();

	void buildtree(int x, int y, int z);
}
