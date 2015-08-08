#include "World.h"
#include "Textures.h"
#include "Renderer.h"
#include "WorldGen.h"
#include <fstream>
extern bool UseCIArray;
extern int viewdistance;
namespace world{

	string worldname = "DefaultWorld";
	brightness skylight = 15;         //Sky light level;
	brightness BRIGHTNESSMAX = 15;    //Maximum brightness;
	brightness BRIGHTNESSMIN = 2;     //Mimimum brightness;
	brightness BRIGHTNESSDEC = 1;    //Brightness decree;
	uint EmptyList;
	chunk* chunks;
	int loadedChunks;
	int ciCacheIndex = -2;
	uint64 ciCacheID = 0;
	chunkIndexArray ciArray;
	bool ciArrayAval;
	sbyte cloud[256][256];
	int rebuiltChunks, rebuiltChunksCount;
	int updatedChunks, updatedChunksCount;
	int unloadedChunks, unloadedChunksCount;
	//int chunkRenderList[65536][4];
	pair<int, chunk*> chunkRenderList[65536];
	int chunkLoadList[65536][4];
	int chunkUnloadList[65536][4];
	//pair<int,chunk*> chunkUnloadList[65536];
	vector<int> displayListUnloadList;
	int chunkRenders, chunkLoads, chunkUnloads;

	vector<shared_ptr<Mo>> MOs;

	void Init(){

		glNewList(EmptyList, GL_COMPILE); glEndList();
		loadedChunks = 0;
		if (chunks != nullptr){
			free(chunks);
			chunks = nullptr;
		}
		rebuiltChunks = 0; rebuiltChunksCount = 0;
		updatedChunks = 0; updatedChunksCount = 0;
		unloadedChunks = 0; unloadedChunksCount = 0;
		memset(chunkRenderList, 0, sizeof(chunkRenderList));
		memset(chunkLoadList, 0, sizeof(chunkLoadList));
		memset(chunkUnloadList, 0, sizeof(chunkUnloadList));
		chunkRenders = 0; chunkLoads = 0; chunkUnloads = 0;
		std::stringstream ss;
		system("mkdir Worlds");
		ss << "mkdir Worlds\\" << worldname;
		system(ss.str().c_str());
		ss.clear(); ss.str("");
		ss << "mkdir Worlds\\" << worldname << "\\chunks";
		system(ss.str().c_str());
		WorldGen::perlinNoiseInit(12221222);
		ciCacheIndex = -2;
		ciCacheID = 0;
		if (ciArrayAval) ciArray.destroy();
		if (UseCIArray){
			ciArray.setsize((viewdistance + 2) * 2);
			ciArrayAval = !ciArray.create();
			if (ciArrayAval == false){
				printf("[console][Warning]");
				printf("ChunkIndexArray not avaliable because it couldn't be created.\n");
			}
		}

	}

	void AddChunk(int x, int y, int z){

		int first, last, middle, i;
		uint64 cid;
		cid = getChunkID(x, y, z);                                                   //Chunk ID;
		//二分查找,GO!;
		first = 0;
		last = loadedChunks - 1;
		middle = int((first + last) / 2);
		if (loadedChunks > 0){
			while (first <= last && chunks[middle].id != cid){
				if (chunks[middle].id > cid) last = middle - 1;
				if (chunks[middle].id < cid) first = middle + 1;
				middle = int((first + last) / 2);
			}
			if (chunks[middle].id == cid){
				printf("[Console][Error]");
				printf("Chunk(%d,%d,%d)has been loaded,when adding chunk.\n", x, y, z);
			}
		}
		ExpandChunkArray(1);
		for (i = loadedChunks - 1; i >= first + 1; i--){
			chunks[i] = chunks[i - 1];
		}
		chunks[first].Init(x, y, z, cid);
		if (ciCacheID >= cid){
			ciCacheID = 0;
			ciCacheIndex = -2;
		}
		ciArray.AddChunk(first);

	}

	void DeleteChunk(int x, int y, int z){
		int index = getChunkIndex(x, y, z);
		if (index!=-1){
			int i;
			for (i = index; i < loadedChunks - 1; i++){
				chunks[i] = chunks[i + 1];
			}
			chunks[loadedChunks - 1] = chunk();
			ReduceChunkArray(1);
			if (ciCacheID >= index){
				ciCacheID = 0;
				ciCacheIndex = -2;
			}
			ciArray.DeleteChunk(index);
		}
		else{
			printf("[console][Warning]");
			printf("Chunk(%d,%d,%d)is not exist,when deleting chunk.\n", x, y, z);
		}
	}

	uint64 getChunkID(int x, int y, int z){
		
		//x = -134217728 ~ 134217727      (28 bits);
		//y = -128       ~ 127            (8 bits);
		//z = -134217728 ~ 134217727      (28 bits);
		uint64 cid;
		bool yNeg=false, xNeg=false, zNeg=false;
		if (y < 0){
			yNeg = true; y = abs(y);
		}
		if (x < 0){
			xNeg = true; x = abs(x);
		}
		if (z < 0){
			zNeg = true; z = abs(z);
		}
		if (y == 128) y = 0;                //y = -128;
		if (x == 134217728) x = 0;          //x = -134217728;
		if (z == 134217728) z = 0;          //z = -134217738;
		cid = (uint64(y) << 56) + (uint64(x) << 28) + (uint64(z));
		if (yNeg) cid += uint64(2) << 62;
		if (xNeg) cid += uint64(2) << 54;
		if (zNeg) cid += uint64(2) << 26;
		return cid;

	}

	bool chunkLoaded(int x, int y, int z){

		bool ret = false;
		if (!chunkOutOfBound(x, y, z)){
			int i;
			i = getChunkIndex(x, y, z);
			if (i != -1) ret = true;
		}
		return ret;

	}

	bool chunkMatched(chunk c, int x, int y, int z){
		return c.cx == x && c.cy == y && c.cz == z;
	}

	int getChunkIndex(int x, int y, int z){
		uint64 cID = getChunkID(x, y, z);
		int ret;
		if (ciCacheID == cID && ciCacheIndex != -2){
			ret = ciCacheIndex;
		}
		else{
			if (UseCIArray && world::ciArrayAval && ciArray.chunkIndexExists(x, y, z)){
				ret = ciArray.getChunkIndex(x, y, z);
				ciCacheID = cID;
				ciCacheIndex = ret;
			}
			else{
				int first, last, middle;
				uint64 cid;
				cid = cID;                                           //Chunk ID;
				ret = -1;
				//二分查找,GO!;
				first = 0;
				last = loadedChunks - 1;
				middle = (first + last) / 2;
				if (loadedChunks > 0){
					while (first <= last){
						uint64 cidnow = chunks[middle].id;
						if (cidnow == cid) break;
						if (cidnow > cid) last = middle - 1;
						else first = middle + 1;
						middle = int((first + last) / 2);
					}
					if (first <= last){
						ret = middle;
						ciCacheID = cid;
						ciCacheIndex = middle;
						if (UseCIArray&&world::ciArrayAval && ciArray.elementExists(x - ciArray.originX, y - ciArray.originY, z - ciArray.originZ)){
							ciArray.Array[(x - ciArray.originX)*ciArray.size2 + (y - ciArray.originY)*ciArray.size + (z - ciArray.originZ)] = middle;
						}
					}
				}
			}
		}
		return ret;

	}

	chunk* getChunkPtr(int x, int y, int z, bool silentMode){

		chunk* ret = nullptr;
		if (chunkOutOfBound(x, y, z)) return ret;
		int i = getChunkIndex(x, y, z);
		if (i != -1){
			ret = &chunks[i];
		}
		else{
			if (!silentMode){
				printf("[Console][Error]");
				printf("Chunk(%d,%d,%d)is not exist,when getting pointer.\n", x, y, z);
			}
		}
		return ret;

	}

	void ExpandChunkArray(int cc){

		loadedChunks += cc;
		chunks = (chunk*)realloc(chunks, loadedChunks * sizeof(chunk));
		if (chunks == nullptr && loadedChunks!= 0){
			printf("[Console][Error]");
			printf("Chunk Array expanding error.\n");
			glfwTerminate();
			saveAllChunks();
			destroyAllChunks();
			glfwTerminate();
			exit(0);
		}

	}

	void ReduceChunkArray(int cc){

		loadedChunks -= cc;
		chunks = (chunk*)realloc(chunks, loadedChunks *sizeof(chunk));
		if (chunks == nullptr && loadedChunks >0){
			printf("[Console][Error]");
			printf("Chunk Array reducing error.\n");
			saveAllChunks();
			destroyAllChunks();
			glfwTerminate();
			exit(0);
		}
		

	}

	void renderblock(int x, int y, int z,int cx,int cy,int cz) {

		//渲染方块！！！！！
		int gx = cx * 16 + x, gy = cy * 16 + y, gz = cz * 16 + z;
		double colors, color1, color2, color3, color4, tcx, tcy, size;
		block blk[7] = { getblock(gx, gy, gz),
			getblock(gx, gy, gz + 1, blocks::ROCK),
			getblock(gx, gy, gz - 1, blocks::ROCK),
			getblock(gx + 1, gy, gz, blocks::ROCK),
			getblock(gx - 1, gy, gz, blocks::ROCK),
			getblock(gx, gy + 1, gz, blocks::ROCK),
			getblock(gx, gy - 1, gz, blocks::ROCK) };
		auto b01n1 = getblock(gx, gy + 1, gz - 1);
		auto b011 = getblock(gx, gy + 1, gz + 1);
		auto b0n11 = getblock(gx, gy - 1, gz + 1);
		auto b0n1n1 = getblock(gx, gy - 1, gz - 1);
		auto b111 = getblock(gx + 1, gy + 1, gz + 1);
		auto b11n1 = getblock(gx + 1, gy + 1, gz - 1);
		auto b1n11 = getblock(gx + 1, gy - 1, gz + 1);
		auto b1n1n1 = getblock(gx + 1, gy - 1, gz - 1);
		auto bn111 = getblock(gx - 1, gy + 1, gz + 1);
		auto bn11n1 = getblock(gx - 1, gy + 1, gz - 1);
		auto bn1n11 = getblock(gx - 1, gy - 1, gz + 1);
		auto bn1n1n1 = getblock(gx - 1, gy - 1, gz - 1);
		auto b10n1 = getblock(gx + 1, gy, gz - 1);
		auto b101 = getblock(gx + 1, gy, gz + 1);
		auto b110 = getblock(gx + 1, gy + 1, gz);
		auto b1n10 = getblock(gx + 1, gy - 1, gz);
		auto bn101 = getblock(gx - 1, gy, gz + 1);
		auto bn10n1 = getblock(gx - 1, gy, gz - 1);
		auto bn110 = getblock(gx - 1, gy + 1, gz);
		auto bn1n10 = getblock(gx - 1, gy - 1, gz);
		brightness brt[7] = { getbrightness(gx, gy, gz),
			getbrightness(gx, gy, gz + 1),
			getbrightness(gx, gy, gz - 1),
			getbrightness(gx + 1, gy, gz),
			getbrightness(gx - 1, gy, gz),
			getbrightness(gx, gy + 1, gz),
			getbrightness(gx, gy - 1, gz) };

		size = BLOCKTEXTURE_UNITSIZE / (double)BLOCKTEXTURE_SIZE - 0.001;

		if (b0n11 == blocks::GRASS && blk[0] == blocks::GRASS)
			tcx = textures::getTexcoordX(blk[0], 1) + 0.001;
		else
			tcx = textures::getTexcoordX(blk[0], 2) + 0.001;

		if (b0n11 == blocks::GRASS && blk[0] == blocks::GRASS)
			tcy = textures::getTexcoordY(blk[0], 1) + 0.001;
		else
			tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Front Face;
		if (!(BlockInfo(blk[1]).isOpaque() || (blk[1] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[1] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 0.5;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(b0n11).isDark()) { color1 = colors*0.5; color2 = colors*0.5; }
				if (BlockInfo(b011).isDark()) { color3 = colors*0.5; color4 = colors*0.5; }
				if (BlockInfo(bn101).isDark()) { color1 = colors*0.5; color4 = colors*0.5; }
				if (BlockInfo(b101).isDark()) { color2 = colors*0.5; color3 = colors*0.5; }

				if (BlockInfo(bn1n11).isDark()) color1 = colors*0.5;
				if (BlockInfo(b1n11).isDark()) color2 = colors*0.5;
				if (BlockInfo(b111).isDark()) color3 = colors*0.5;
				if (BlockInfo(bn111).isDark()) color4 = colors*0.5;

			}
			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);

		}

		if (b0n1n1 == blocks::GRASS && blk[0] == blocks::GRASS)
			tcx = textures::getTexcoordX(blk[0], 1) + 0.001;
		else
			tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (b0n1n1 == blocks::GRASS && blk[0] == blocks::GRASS)
			tcy = textures::getTexcoordY(blk[0], 1) + 0.001;
		else
			tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Back Face;
		if (!(BlockInfo(blk[2]).isOpaque() || (blk[2] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[2] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 0.5;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(b0n1n1).isDark()) {
					color1 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(b01n1).isDark()) {
					color2 = colors*0.5; color3 = colors*0.5;
				}
				if (BlockInfo(bn10n1).isDark()) {
					color1 = colors*0.5; color2 = colors*0.5;
				}
				if (BlockInfo(b10n1).isDark()) {
					color3 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(bn1n1n1).isDark()) color1 = colors*0.5;
				if (BlockInfo(bn11n1).isDark()) color2 = colors*0.5;
				if (BlockInfo(b11n1).isDark()) color3 = colors*0.5;
				if (BlockInfo(b1n1n1).isDark()) color4 = colors*0.5;

			}

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);

		}

		if (b1n10 == blocks::GRASS && blk[0] == blocks::GRASS) tcx = textures::getTexcoordX(blk[0], 1) + 0.001; else tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (b1n10 == blocks::GRASS && blk[0] == blocks::GRASS) tcy = textures::getTexcoordY(blk[0], 1) + 0.001; else tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Right face;
		if (!(BlockInfo(blk[3]).isOpaque() || (blk[3] == blk[0] && !BlockInfo(blk[0]).isOpaque())) || blk[0] == blocks::LEAF) {

			colors = brt[3] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 0.7;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(b1n10).isDark()) {
					color1 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(b110).isDark()) {
					color2 = colors*0.5; color3 = colors*0.5;
				}
				if (BlockInfo(b10n1).isDark()) {
					color1 = colors*0.5; color2 = colors*0.5;
				}
				if (BlockInfo(b101).isDark()) {
					color3 = colors*0.5; color4 = colors*0.5;
				}

				if (BlockInfo(b1n1n1).isDark()) color1 = colors*0.5;
				if (BlockInfo(b11n1).isDark()) color2 = colors*0.5;
				if (BlockInfo(b111).isDark()) color3 = colors*0.5;
				if (BlockInfo(getblock(x + 1, y - 1, z + 1)).isDark()) color4 = colors*0.5;

			}

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);

		}

		if (bn1n10 == blocks::GRASS && blk[0] == blocks::GRASS) tcx = textures::getTexcoordX(blk[0], 1) + 0.001; else tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (bn1n10 == blocks::GRASS && blk[0] == blocks::GRASS) tcy = textures::getTexcoordY(blk[0], 1) + 0.001; else tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Left Face;
		if (!(BlockInfo(blk[4]).isOpaque() || (blk[4] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[4] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 0.7;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(bn1n10).isDark()) {
					color1 = colors*0.5; color2 = colors*0.5;
				}
				if (BlockInfo(bn110).isDark()) {
					color3 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(bn10n1).isDark()) {
					color1 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(bn101).isDark()) {
					color2 = colors*0.5; color3 = colors*0.5;
				}

				if (BlockInfo(bn1n1n1).isDark()) color1 = colors*0.5;
				if (BlockInfo(bn1n11).isDark()) color2 = colors*0.5;
				if (BlockInfo(bn111).isDark()) color3 = colors*0.5;
				if (BlockInfo(bn11n1).isDark()) color4 = colors*0.5;

			}

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);

		}

		tcx = textures::getTexcoordX(blk[0], 1);
		tcy = textures::getTexcoordY(blk[0], 1);

		// Top Face;
		if (!(BlockInfo(blk[5]).isOpaque() || (blk[5] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[5] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 1.0;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(b01n1).isDark()) {
					color1 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(b011).isDark()) {
					color2 = colors*0.5; color3 = colors*0.5;
				}
				if (BlockInfo(bn110).isDark()) {
					color1 = colors*0.5; color2 = colors*0.5;
				}
				if (BlockInfo(b110).isDark()) {
					color3 = colors*0.5; color4 = colors*0.5;
				}

				if (BlockInfo(bn11n1).isDark()) color1 = colors*0.5;
				if (BlockInfo(bn111).isDark()) color2 = colors*0.5;
				if (BlockInfo(b111).isDark()) color3 = colors*0.5;
				if (BlockInfo(b11n1).isDark()) color4 = colors*0.5;

			}

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);

		}

		tcx = textures::getTexcoordX(blk[0], 3);
		tcy = textures::getTexcoordY(blk[0], 3);

		// Bottom Face;
		if (!(BlockInfo(blk[6]).isOpaque() || (blk[6] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[6] / (double)BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) colors *= 1.0;
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE) {

				if (BlockInfo(b0n1n1).isDark()) {
					color1 = colors*0.5; color2 = colors*0.5;
				}
				if (BlockInfo(b0n11).isDark()) {
					color3 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(bn1n10).isDark()) {
					color1 = colors*0.5; color4 = colors*0.5;
				}
				if (BlockInfo(b1n10).isDark()) {
					color2 = colors*0.5; color3 = colors*0.5;
				}

				if (BlockInfo(bn1n1n1).isDark()) color1 = colors*0.5;
				if (BlockInfo(b1n1n1).isDark()) color2 = colors*0.5;
				if (BlockInfo(getblock(x + 1, y - 1, z + 1)).isDark()) color3 = colors*0.5;
				if (BlockInfo(bn1n11).isDark()) color4 = colors*0.5;

			}

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);

		}
	}
	
	void renderblock(int x, int y, int z, chunk* chunkptr) {

		//渲染方块v2！！！！！

		double colors, color1, color2, color3, color4, tcx, tcy, size;
		int cx = chunkptr->cx, cy = chunkptr->cy, cz = chunkptr->cz;
		int gx = cx * 16 + x, gy = cy * 16 + y, gz = cz * 16 + z;
		block blk[7] = { chunkptr->getblock(x,y,z) ,
			z < 15 ? chunkptr->getblock(x,y,z + 1) : getblock(gx,gy,gz + 1,blocks::STONE),
			z>0 ? chunkptr->getblock(x,y,z - 1) : getblock(gx,gy,gz - 1,blocks::STONE),
			x < 15 ? chunkptr->getblock(x + 1,y,z) : getblock(gx + 1,gy,gz,blocks::STONE),
			x>0 ? chunkptr->getblock(x - 1,y,z) : getblock(gx - 1,gy,gz,blocks::STONE),
			y < 15 ? chunkptr->getblock(x,y + 1,z) : getblock(gx,gy + 1,gz,blocks::STONE),
			y>0 ? chunkptr->getblock(x,y - 1,z) : getblock(gx,gy - 1,gz,blocks::STONE) };

		brightness brt[7] = { chunkptr->getbrightness(x,y,z) ,
			z < 15 ? chunkptr->getbrightness(x,y,z + 1) : getbrightness(gx,gy,gz + 1),
			z>0 ? chunkptr->getbrightness(x,y,z - 1) : getbrightness(gx,gy,gz - 1),
			x < 15 ? chunkptr->getbrightness(x + 1,y,z) : getbrightness(gx + 1,gy,gz),
			x>0 ? chunkptr->getbrightness(x - 1,y,z) : getbrightness(gx - 1,gy,gz),
			y < 15 ? chunkptr->getbrightness(x,y + 1,z) : getbrightness(gx,gy + 1,gz),
			y>0 ? chunkptr->getbrightness(x,y - 1,z) : getbrightness(gx,gy - 1,gz) };

		size = BLOCKTEXTURE_UNITSIZE / (double)BLOCKTEXTURE_SIZE - 0.001;

		if (getblock(gx, gy - 1, gz + 1) == blocks::GRASS && blk[0] == blocks::GRASS)
			tcx = textures::getTexcoordX(blk[0], 1) + 0.001;
		else
			tcx = textures::getTexcoordX(blk[0], 2) + 0.001;

		if (getblock(gx, gy - 1, gz + 1) == blocks::GRASS && blk[0] == blocks::GRASS)
			tcy = textures::getTexcoordY(blk[0], 1) + 0.001;
		else
			tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Front Face;
		if (!(BlockInfo(blk[1]).isOpaque() || (blk[1] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[1];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (colors + getbrightness(gx, gy - 1, gz + 1) + getbrightness(gx - 1, gy, gz + 1) + getbrightness(gx - 1, gy - 1, gz + 1)) / 4.0;
				color2 = (colors + getbrightness(gx, gy - 1, gz + 1) + getbrightness(gx + 1, gy, gz + 1) + getbrightness(gx + 1, gy - 1, gz + 1)) / 4.0;
				color3 = (colors + getbrightness(gx, gy + 1, gz + 1) + getbrightness(gx + 1, gy, gz + 1) + getbrightness(gx + 1, gy + 1, gz + 1)) / 4.0;
				color4 = (colors + getbrightness(gx, gy + 1, gz + 1) + getbrightness(gx - 1, gy, gz + 1) + getbrightness(gx - 1, gy + 1, gz + 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color1 *= 0.5;
			color2 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color2 *= 0.5;
			color3 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color3 *= 0.5;
			color4 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color4 *= 0.5;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);

		}

		if (getblock(gx, gy - 1, gz - 1) == blocks::GRASS && blk[0] == blocks::GRASS)
			tcx = textures::getTexcoordX(blk[0], 1) + 0.001;
		else
			tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (getblock(gx, gy - 1, gz - 1) == blocks::GRASS && blk[0] == blocks::GRASS)
			tcy = textures::getTexcoordY(blk[0], 1) + 0.001;
		else
			tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Back Face;
		if (!(BlockInfo(blk[2]).isOpaque() || (blk[2] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[2];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (colors + getbrightness(gx, gy - 1, gz - 1) + getbrightness(gx - 1, gy, gz - 1) + getbrightness(gx - 1, gy - 1, gz - 1)) / 4.0;
				color2 = (colors + getbrightness(gx, gy + 1, gz - 1) + getbrightness(gx - 1, gy, gz - 1) + getbrightness(gx - 1, gy + 1, gz - 1)) / 4.0;
				color3 = (colors + getbrightness(gx, gy + 1, gz - 1) + getbrightness(gx + 1, gy, gz - 1) + getbrightness(gx + 1, gy + 1, gz - 1)) / 4.0;
				color4 = (colors + getbrightness(gx, gy - 1, gz - 1) + getbrightness(gx + 1, gy, gz - 1) + getbrightness(gx + 1, gy - 1, gz - 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color1 *= 0.5;
			color2 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color2 *= 0.5;
			color3 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color3 *= 0.5;
			color4 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color4 *= 0.5;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);

		}

		if (getblock(gx - 1, gy - 1, gz) == blocks::GRASS && blk[0] == blocks::GRASS) tcx = textures::getTexcoordX(blk[0], 1) + 0.001; else tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (getblock(gx - 1, gy - 1, gz) == blocks::GRASS && blk[0] == blocks::GRASS) tcy = textures::getTexcoordY(blk[0], 1) + 0.001; else tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Right face;
		if (!(BlockInfo(blk[3]).isOpaque() || (blk[3] == blk[0] && !BlockInfo(blk[0]).isOpaque())) || blk[0] == blocks::LEAF) {

			colors = brt[3];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (colors + getbrightness(gx + 1, gy - 1, gz) + getbrightness(gx + 1, gy, gz - 1) + getbrightness(gx + 1, gy - 1, gz - 1)) / 4.0;
				color2 = (colors + getbrightness(gx + 1, gy + 1, gz) + getbrightness(gx + 1, gy, gz - 1) + getbrightness(gx + 1, gy + 1, gz - 1)) / 4.0;
				color3 = (colors + getbrightness(gx + 1, gy + 1, gz) + getbrightness(gx + 1, gy, gz + 1) + getbrightness(gx + 1, gy + 1, gz + 1)) / 4.0;
				color4 = (colors + getbrightness(gx + 1, gy - 1, gz) + getbrightness(gx + 1, gy, gz + 1) + getbrightness(gx + 1, gy - 1, gz + 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color1 *= 0.7;
			color2 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color2 *= 0.7;
			color3 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color3 *= 0.7;
			color4 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color4 *= 0.7;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);

		}

		if (getblock(gx - 1, gy - 1, gz) == blocks::GRASS && blk[0] == blocks::GRASS) tcx = textures::getTexcoordX(blk[0], 1) + 0.001; else tcx = textures::getTexcoordX(blk[0], 2) + 0.001;
		if (getblock(gx - 1, gy - 1, gz) == blocks::GRASS && blk[0] == blocks::GRASS) tcy = textures::getTexcoordY(blk[0], 1) + 0.001; else tcy = textures::getTexcoordY(blk[0], 2) + 0.001;

		// Left Face;
		if (!(BlockInfo(blk[4]).isOpaque() || (blk[4] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[4];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (colors + getbrightness(gx - 1, gy - 1, gz) + getbrightness(gx - 1, gy, gz - 1) + getbrightness(gx - 1, gy - 1, gz - 1)) / 4.0;
				color2 = (colors + getbrightness(gx - 1, gy - 1, gz) + getbrightness(gx - 1, gy, gz + 1) + getbrightness(gx - 1, gy - 1, gz + 1)) / 4.0;
				color3 = (colors + getbrightness(gx - 1, gy + 1, gz) + getbrightness(gx - 1, gy, gz + 1) + getbrightness(gx - 1, gy + 1, gz + 1)) / 4.0;
				color4 = (colors + getbrightness(gx - 1, gy + 1, gz) + getbrightness(gx - 1, gy, gz - 1) + getbrightness(gx - 1, gy + 1, gz - 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color1 *= 0.7;
			color2 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color2 *= 0.7;
			color3 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color3 *= 0.7;
			color4 /= BRIGHTNESSMAX;
			if (blk[0] != blocks::GLOWSTONE) color4 *= 0.7;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);

		}

		tcx = textures::getTexcoordX(blk[0], 1);
		tcy = textures::getTexcoordY(blk[0], 1);

		// Top Face;
		if (!(BlockInfo(blk[5]).isOpaque() || (blk[5] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[5];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (color1 + getbrightness(gx, gy + 1, gz - 1) + getbrightness(gx - 1, gy + 1, gz) + getbrightness(gx - 1, gy + 1, gz - 1)) / 4.0;
				color2 = (color2 + getbrightness(gx, gy + 1, gz + 1) + getbrightness(gx - 1, gy + 1, gz) + getbrightness(gx - 1, gy + 1, gz + 1)) / 4.0;
				color3 = (color3 + getbrightness(gx, gy + 1, gz + 1) + getbrightness(gx + 1, gy + 1, gz) + getbrightness(gx + 1, gy + 1, gz + 1)) / 4.0;
				color4 = (color4 + getbrightness(gx, gy + 1, gz - 1) + getbrightness(gx + 1, gy + 1, gz) + getbrightness(gx + 1, gy + 1, gz - 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			color2 /= BRIGHTNESSMAX;
			color3 /= BRIGHTNESSMAX;
			color4 /= BRIGHTNESSMAX;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, 0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, 0.5 + y, -0.5 + z);

		}

		tcx = textures::getTexcoordX(blk[0], 3);
		tcy = textures::getTexcoordY(blk[0], 3);

		// Bottom Face;
		if (!(BlockInfo(blk[6]).isOpaque() || (blk[6] == blk[0] && BlockInfo(blk[0]).isOpaque() == false)) || blk[0] == blocks::LEAF) {

			colors = brt[6];
			color1 = colors; color2 = colors; color3 = colors; color4 = colors;

			if (blk[0] != blocks::GLOWSTONE && SmoothLighting) {

				color1 = (colors + getbrightness(gx, gy - 1, gz - 1) + getbrightness(gx - 1, gy - 1, gz) + getbrightness(gx - 1, gy - 1, gz - 1)) / 4.0;
				color2 = (colors + getbrightness(gx, gy - 1, gz - 1) + getbrightness(gx + 1, gy - 1, gz) + getbrightness(gx + 1, gy - 1, gz - 1)) / 4.0;
				color3 = (colors + getbrightness(gx, gy - 1, gz + 1) + getbrightness(gx + 1, gy - 1, gz) + getbrightness(gx + 1, gy - 1, gz + 1)) / 4.0;
				color4 = (colors + getbrightness(gx, gy - 1, gz + 1) + getbrightness(gx - 1, gy - 1, gz) + getbrightness(gx - 1, gy - 1, gz + 1)) / 4.0;

			}

			color1 /= BRIGHTNESSMAX;
			color2 /= BRIGHTNESSMAX;
			color3 /= BRIGHTNESSMAX;
			color4 /= BRIGHTNESSMAX;

			renderer::Color3d(color1, color1, color1);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*1.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color2, color2, color2);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*1.0); renderer::Vertex3d(0.5 + x, -0.5 + y, -0.5 + z);
			renderer::Color3d(color3, color3, color3);
			renderer::TexCoord2d(tcx + size*0.0, tcy + size*0.0); renderer::Vertex3d(0.5 + x, -0.5 + y, 0.5 + z);
			renderer::Color3d(color4, color4, color4);
			renderer::TexCoord2d(tcx + size*1.0, tcy + size*0.0); renderer::Vertex3d(-0.5 + x, -0.5 + y, 0.5 + z);

		}
	}

	int getchunkpos(int n){

		//给出N对应的chunk坐标，XYZ通用;
		int ret = 0;
		if (n >= 0){
			ret = n >> 4;
		}
		else if (n<0){
			ret = int((n - 15) / 16);
		}
		return ret;

	}

	int getblockpos(int n){
		//给出N对应的chunk内的方块坐标，XYZ通用;
		return n & 0xf;
	}

	vector<Hitbox::AABB> getHitboxes(Hitbox::AABB box){
		//返回与box相交的所有方块AABB

		Hitbox::AABB blockbox;
		vector<Hitbox::AABB> hitBoxes;

		for (int a = int(box.xmin + 0.5) - 1; a <= int(box.xmax + 0.5) + 1; a++){
			for (int b = int(box.ymin + 0.5) - 1; b <= int(box.ymax + 0.5) + 1; b++){
				for (int c = int(box.zmin + 0.5) - 1; c <= int(box.zmax + 0.5) + 1; c++){ //? check
					if (BlockInfo(getblock(a, b, c)).isSolid()){
						blockbox.xmin = a - 0.5;
						blockbox.xmax = a + 0.5;
						blockbox.ymin = b - 0.5;
						blockbox.ymax = b + 0.5;
						blockbox.zmin = c - 0.5;
						blockbox.zmax = c + 0.5;
						if (Hitbox::Hit(box, blockbox)){
							hitBoxes.push_back(blockbox);
						}
					}
				}
			}
		}
		return hitBoxes;
	}

	bool inWater(Hitbox::AABB box){
		Hitbox::AABB blockbox;
		int a, b, c;
		for (a = int(box.xmin + 0.5) - 1; a != int(box.xmax + 0.5); a++){
			for (b = int(box.ymin + 0.5) - 1; b != int(box.ymax + 0.5); b++){
				for (c = int(box.zmin + 0.5) - 1; c != int(box.zmax + 0.5); c++){
					if (getblock(a, b, c) == blocks::WATER) {
						blockbox.xmin = a - 0.5;
						blockbox.xmax = a + 0.5;
						blockbox.ymin = b - 0.5;
						blockbox.ymax = b + 0.5;
						blockbox.zmin = c - 0.5;
						blockbox.zmax = c + 0.5;
						if (Hitbox::Hit(box, blockbox)) return true;
					}
				}
			}
		}
		return false;
	}

	void updateblock(int x, int y, int z, bool blockchanged){
		//Blockupdate;

		int cx, cy, cz, bx, by, bz;
		bool updated = blockchanged;

		cx = getchunkpos(x);
		cy = getchunkpos(y);
		cz = getchunkpos(z);

		bx = getblockpos(x);
		by = getblockpos(y);
		bz = getblockpos(z);

		if (chunkOutOfBound(cx, cy, cz) == false){

			auto cptr = getChunkPtr(cx, cy, cz, true);
			if (cptr!=nullptr){
				brightness oldbrightness = cptr->getbrightness(bx, by, bz);
				bool skylighted = true;
				int yi, cyi;
				yi = y + 1; cyi = getchunkpos(yi);
				if (y < 0){
					skylighted = false;
				}
				else{
					while (chunkLoaded(cx, cyi + 1, cz) && skylighted){
						if (BlockInfo(getblock(x, yi, z)).isOpaque() || getblock(x, yi, z) == blocks::WATER){
							skylighted = false;
						}
						yi++; cyi = getchunkpos(yi);
					}
				}
				if (!BlockInfo(getblock(x, y, z)).isOpaque()){

					brightness br;
					int maxbrightness;
					block blks[7] = { 0,
						getblock(x, y, z + 1),    //Front face;
						getblock(x, y, z - 1),    //Back face;
						getblock(x + 1, y, z),    //Right face;
						getblock(x - 1, y, z),    //Left face;
						getblock(x, y + 1, z),    //Top face;
						getblock(x, y - 1, z) };     //Bottom face;
					brightness brts[7] = { 0,
						getbrightness(x, y, z + 1),    //Front face;
						getbrightness(x, y, z - 1),    //Back face;
						getbrightness(x + 1, y, z),    //Right face;
						getbrightness(x - 1, y, z),    //Left face;
						getbrightness(x, y + 1, z),    //Top face;
						getbrightness(x, y - 1, z) };     //Bottom face;
					maxbrightness = 1;
					for (int i = 2; i != 6; i++){
						if (brts[maxbrightness] < brts[i]) maxbrightness = i;
					}
					br = brts[maxbrightness];
					if (blks[maxbrightness] == blocks::WATER){
						if (br - 2 < BRIGHTNESSMIN) br = BRIGHTNESSMIN; else br -= 2;
					}
					else{
						if (br - 1 < BRIGHTNESSMIN) br = BRIGHTNESSMIN; else br--;
					}

					if (skylighted){
						if (br < skylight) br = skylight;
					}
					if (br < BRIGHTNESSMIN) br = BRIGHTNESSMIN;
					//Set brightness;
					cptr->setbrightness(bx, by, bz, br);

				}
				else{

					//Opaque block;
					getChunkPtr(cx, cy, cz)->setbrightness(bx, by, bz, 0);
					if (getblock(x, y, z) == blocks::GLOWSTONE || getblock(x, y, z) == blocks::LAVA){
						cptr->setbrightness(bx, by, bz, BRIGHTNESSMAX);
					}

				}


				if (oldbrightness != cptr->getbrightness(bx, by, bz)) updated = true;

				if (updated){
					updateblock(x, y + 1, z, false);
					updateblock(x, y - 1, z, false);
					updateblock(x + 1, y, z, false);
					updateblock(x - 1, y, z, false);
					updateblock(x, y, z + 1, false);
					updateblock(x, y, z - 1, false);
				}

				setChunkUpdated(cx, cy, cz, true);
				if (bx == 15 && cx<worldsize - 1) setChunkUpdated(cx + 1, cy, cz, true);
				if (bx == 0 && cx>-worldsize) setChunkUpdated(cx - 1, cy, cz, true);
				if (by == 15 && cy<worldheight - 1) setChunkUpdated(cx, cy + 1, cz, true);
				if (by == 0 && cy>-worldheight) setChunkUpdated(cx, cy - 1, cz, true);
				if (bz == 15 && cz<worldsize - 1) setChunkUpdated(cx, cy, cz + 1, true);
				if (bz == 0 && cz>-worldsize) setChunkUpdated(cx, cy, cz - 1, true);

			}
		}
	}

	block getblock(int x, int y, int z, block defaultRet){

		//获取XYZ的方块;
		block ret = defaultRet;
		int cx, cy, cz, bx, by, bz;

		cx = getchunkpos(x);
		cy = getchunkpos(y);
		cz = getchunkpos(z);

		bx = getblockpos(x);
		by = getblockpos(y);
		bz = getblockpos(z);

		auto chunkptr = getChunkPtr(cx, cy, cz, true);
		if (chunkptr == nullptr) return ret;
		ret = chunkptr->getblock(bx, by, bz);

		return ret;
	}

	brightness getbrightness(int x, int y, int z){

		//获取XYZ的亮度;
		brightness ret = skylight;
		int cx, cy, cz, bx, by, bz;

		cx = getchunkpos(x);
		cy = getchunkpos(y);
		cz = getchunkpos(z);

		bx = getblockpos(x);
		by = getblockpos(y);
		bz = getblockpos(z);

		auto chunkptr = getChunkPtr(cx, cy, cz, true);
		if (chunkptr == nullptr) return ret;
		ret = chunkptr->getbrightness(bx, by, bz);


		return ret;

	}

	void setblock(int x, int y, int z, block Blockname){

		//设置方块;
		int cx, cy, cz, bx, by, bz;

		cx = getchunkpos(x);
		cy = getchunkpos(y);
		cz = getchunkpos(z);

		bx = getblockpos(x);
		by = getblockpos(y);
		bz = getblockpos(z);

		auto cptr = getChunkPtr(cx, cy, cz, true);
		if (cptr == nullptr) return;
		cptr->setblock(bx, by, bz, Blockname);
		updateblock(x, y, z, true);

	}

	void setbrightness(int x, int y, int z, brightness tbrightness){

		//设置XYZ的亮度;
		int cx, cy, cz, bx, by, bz;

		cx = getchunkpos(x);
		cy = getchunkpos(y);
		cz = getchunkpos(z);

		bx = getblockpos(x);
		by = getblockpos(y);
		bz = getblockpos(z);

		auto cptr = getChunkPtr(cx, cy, cz, true);
		if (cptr == nullptr) return;
		cptr->setbrightness(bx, by, bz, tbrightness);

	}

	void putblock(int x, int y, int z, block Blockname){

		//这个void和上面那个是一样的，只是本人的完美主义（说白了就是强迫症）驱使我再写一遍= =;
		//是不是感觉这句话有些眼熟。。。;
		setblock(x, y, z, Blockname);

	}

	void pickblock(int x, int y, int z){

		//这个void根本没有存在的必要，其功能等于setblock(x,y,z,0)或putblock(x,y,z,0),但是本人的完...;
		//是不是感觉这句话还是有些眼熟。。。;
		setblock(x, y, z, blocks::AIR);

	}

	//为什么之前那些话都有些眼熟呢？？？;
	//原来是因为这里的set/put/pickblock三个Sub是世界范围的，而之前的是区块范围的。。。;

	bool chunkOutOfBound(int x, int y, int z){

		//检测给出的chunk坐标是否越界;
		bool ret = false;
		if (x<-worldsize || x>worldsize - 1) ret = true;
		if (y<-worldheight || y>worldheight - 1) ret = true;
		if (z<-worldsize || z>worldsize - 1) ret = true;
		return ret;

	}

	bool chunkInRange(int x, int y, int z, int px, int py, int pz, int dist){

		//检测给出的chunk坐标是否在渲染范围内;
		bool ret = true;
		if (x<px - dist || x>px + dist - 1 || y<py - dist || y>py + dist - 1 || z<pz - dist || z>pz + dist - 1){
			ret = false;
		}
		return ret;

	}

	bool chunkUpdated(int x, int y, int z){

		return getChunkPtr(x, y, z)->getUpdated();

	}

	void setChunkUpdated(int x, int y, int z, bool value){

		auto cptr = getChunkPtr(x, y, z, true);
		if (cptr != nullptr) cptr->setUpdated(value);

	}

	void sortChunkRenderList(int xpos, int ypos, int zpos){

		//根据chunk与一个点的距离对chunk的渲染顺序进行排序，离玩家越近的chunk先渲染;
		int cxp, cyp, czp, p=0, i;
		int xd, yd, zd;

		cxp = getchunkpos(xpos);
		cyp = getchunkpos(ypos);
		czp = getchunkpos(zpos);

		for (i = 0; i != loadedChunks; i++){
			if (p >= 65536) {
				printf("[Console][Error]");
				printf("Chunk render list overflow.\n");
				return;
			}

			auto cptr = &world::chunks[i];
			if (cptr->isEmptyChunk || !cptr->getUpdated()) continue;

			p++;

			xd = cptr->cx * 16 + 7 - xpos;
			yd = cptr->cy * 16 + 7 - ypos;
			zd = cptr->cz * 16 + 7 - zpos;
			chunkRenderList[p].first = xd*xd + yd*yd + zd*zd;

			chunkRenderList[p].second = cptr;
		}

		qsortListPtr(chunkRenderList, 1, p, false);

		chunkRenders = p;

	}

	void sortChunkLoadList(int xpos, int ypos, int zpos){

		int cxp, cyp, czp, cx, cy, cz, p=0;
		int xd, yd, zd, distsqr;

		cxp = getchunkpos(xpos);
		cyp = getchunkpos(ypos);
		czp = getchunkpos(zpos);

		for (cx = cxp - viewdistance - 1; cx <= cxp + viewdistance; cx++){
			for (cy = cyp - viewdistance - 1; cy <= cyp + viewdistance; cy++){
				for (cz = czp - viewdistance - 1; cz <= czp + viewdistance; cz++){
					if (!chunkOutOfBound(cx, cy, cz)){
						if (!chunkLoaded(cx, cy, cz)){
							if (p >= 65536){
								printf("[Console][Error]");
								printf("Chunk load list overflow.\n");
								return;
							}
							p++;
							chunkLoadList[p][1] = cx;
							chunkLoadList[p][2] = cy;
							chunkLoadList[p][3] = cz;
							xd = cx * 16 + 7 - xpos;
							yd = cy * 16 + 7 - ypos;
							zd = cz * 16 + 7 - zpos;
							distsqr = xd*xd + yd*yd + zd*zd;
							chunkLoadList[p][0] = distsqr;
						}
					}
				}
			}
		}
		qsortList(chunkLoadList, 1, p, false);
		chunkLoads = p;

	}

	void sortChunkUnloadList(int xpos, int ypos, int zpos){

		int cxp, cyp, czp, ci, cx, cy, cz, p = 0;
		int xd, yd, zd, distsqr;

		cxp = getchunkpos(xpos);
		cyp = getchunkpos(ypos);
		czp = getchunkpos(zpos);

		for (ci = 0; ci < loadedChunks; ci++){
			cx = chunks[ci].cx;
			cy = chunks[ci].cy;
			cz = chunks[ci].cz;
			if (!chunkInRange(cx, cy, cz, cxp, cyp, czp, viewdistance + 1)){
				if (p >= 65536){
					printf("[Console][Error]");
					printf("Chunk unload list overflow.\n");
					return;
				}
				p++;
				
				chunkUnloadList[p][1] = cx;
				chunkUnloadList[p][2] = cy;
				chunkUnloadList[p][3] = cz;
				xd = cx * 16 + 7 - xpos;
				yd = cy * 16 + 7 - ypos;
				zd = cz * 16 + 7 - zpos;
				distsqr = xd*xd + yd*yd + zd*zd;
				chunkUnloadList[p][0] = distsqr;
			}
		}
		qsortList(chunkUnloadList, 1, p, true);

		chunkUnloads = p;

	}
	
	void qsortListPtr(pair<int, chunk*> List[], int first, int last, bool unloadsort) {
		if (first >= last) return;
		int key = List[first].first;
		auto k1 = List[first].second;
		int i = first, j = last;
		while (i != j) {
			if (unloadsort) {
				while (i < j && List[j].first <= key) {
					j -= 1;
				}
			}
			else {
				while (i < j && List[j].first >= key) {
					j -= 1;
				}
			}
			List[i].first = List[j].first;
			List[i].second = List[j].second;
			if (unloadsort) {
				while (i < j && List[j].first >= key) {
					i += 1;
				}
			}
			else {
				while (i < j && List[i].first <= key) {
					i += 1;
				}
			}
			List[j].first = List[i].first;
			List[j].second = List[i].second;
		}
		List[i].first = key;
		List[i].second = k1;
		qsortListPtr(List, first, i - 1, unloadsort);
		qsortListPtr(List, i + 1, last, unloadsort);
	}
	
	void qsortList(int List[][4], int first, int last, bool unloadsort){
		if (first >= last) return;
		int key = List[first][0], k1 = List[first][1], k2 = List[first][2], k3 = List[first][3];
		int i = first, j = last;
		while (i != j){
			if (unloadsort){
				while (i < j && List[j][0] <= key){
					j -= 1;
				}
			}
			else{
				while (i < j && List[j][0] >= key){
					j -= 1;
				}
			}
			List[i][0] = List[j][0];
			List[i][1] = List[j][1];
			List[i][2] = List[j][2];
			List[i][3] = List[j][3];
			if (unloadsort){
				while (i < j && List[j][0] >= key){
					i += 1;
				}
			}
			else{
				while (i < j && List[i][0] <= key){
					i += 1;
				}
			}
			List[j][0] = List[i][0];
			List[j][1] = List[i][1];
			List[j][2] = List[i][2];
			List[j][3] = List[i][3];
		}
		List[i][0] = key;
		List[i][1] = k1;
		List[i][2] = k2;
		List[i][3] = k3;
		qsortList(List, first, i - 1, unloadsort);
		qsortList(List, i + 1, last, unloadsort);
	}

	void saveAllChunks(){

#ifndef DEBUG_NO_FILEIO
		int i;
		for (i = 0; i != loadedChunks ; i++){
			chunks[i].SaveToFile();
		}
#endif
	}

	void destroyAllChunks(){
		int i;
		for (i = 0; i != loadedChunks ; i++){
			chunks[i].destroylists();
			chunks[i].destroy();
		}
		free(chunks);
		chunks = 0;
		loadedChunks = 0;
	}

	void buildtree(int x, int y, int z){

		block trblock = getblock(x, y, z), tublock = getblock(x, y - 1, z);
		ubyte xt, yt, zt;
		ubyte th = ubyte(rnd() * 3) + 4;
		if (trblock != blocks::AIR || tublock != blocks::GRASS || tublock == blocks::AIR) return;

		for (yt = 0; yt != th; yt++){
			setblock(x, y + yt, z, blocks::WOOD);
		}

		setblock(x, y - 1, z, blocks::DIRT);

		for (xt = 0; xt != 4; xt++){
			for (zt = 0; zt != 4; zt++){
				for (yt = 0; yt != 1; yt++){
					if (getblock(x + xt - 2, y + th - 2 - yt, z + zt - 2) == blocks::AIR) setblock(x + xt - 2, y + th - 2 - yt, z + zt - 2, blocks::LEAF);
				}
			}
		}

		for (xt = 0; xt != 2; xt++){
			for (zt = 0; zt != 2; zt++){
				for (yt = 0; yt != 1; yt++){
					if (getblock(x + xt - 1, y + th - 1 + yt, z + zt - 1) == blocks::AIR && abs(xt - 1) != abs(zt - 1)) setblock(x + xt - 1, y + th - 1 + yt, z + zt - 1, blocks::LEAF);
				}
			}
		}

		setblock(x, y + th, z, blocks::LEAF);

	}


}
