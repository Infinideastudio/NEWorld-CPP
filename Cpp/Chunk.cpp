#include "Chunk.h"
#include "WorldGen.h"
#include "World.h"
#include "Renderer.h"
#include <fstream>

namespace world{
	void chunk::Init(int cxi, int cyi, int czi, uint64 idi){

		cx = cxi;
		cy = cyi;
		cz = czi;
		id = idi;
		list = 0;
		Modified = false;
		memset(pblocks, 0, sizeof(pblocks));
		memset(pbrightness, 0, sizeof(pbrightness));
		isEmptyChunk = false;
		isHiddenChunk = false;

	}

	Hitbox::AABB chunk::getChunkAABB(){
		Hitbox::AABB aabb;
		aabb.xmin = cx * 16 - 0.5;
		aabb.ymin = cy * 16 - loadAnim - 0.5;
		aabb.zmin = cz * 16 - 0.5;
		aabb.xmax = cx * 16 + 16 - 0.5;
		aabb.ymax = cy * 16 - loadAnim + 16 - 0.5;
		aabb.zmax = cz * 16 + 16 - 0.5;

		return aabb;
	}
	
	Hitbox::AABB chunk::getRelativeAABB(double x, double y, double z) {
		Hitbox::AABB ret;
		ret.xmin = cx * 16 - 0.5 - x;
		ret.xmax = cx * 16 + 16 - 0.5 - x;
		ret.ymin = cy * 16 - 0.5 - loadAnim - y;
		ret.ymax = cy * 16 + 16 - 0.5 - loadAnim - y;
		ret.zmin = cz * 16 - 0.5 - z;
		ret.zmax = cz * 16 + 16 - 0.5 - z;
		return ret;
	}

	void chunk::create(){
	}

	void chunk::destroy(){
		updated = false;
		unloadedChunks++;
	}

	void chunk::build(){

		//生成地形
		int x, y, z, height, h, sh;
		if (cy < 0) {
			memset(pbrightness, sizeof(pbrightness), BRIGHTNESSMIN);
			isEmptyChunk = true;
			return;
		}

		auto iter = CHMs.find(getChunkID(cx, 0, cz)); //Height Map
		if (iter == CHMs.end()) {
			//没找到可用的HeightMap
			chunkHeightMap CHM(cx, cz); //新建一个
			CHM.build();
			iter = CHMs.insert(CHM).first; //指向新加入的Height Map
		}

		bool EmptyChunk = true;
		for (x = 0; x != 16; x++){
			for (z = 0; z != 16; z++){
				h = iter->terrain[x][z];
				sh = iter->sandTerrain[x][z];
				for (y = 0; y != 16; y++){
					height = cy * 16 + y;
					pbrightness[x][y][z] = 0;
					if (height == 0)
						pblocks[x][y][z] = blocks::BEDROCK;
					else if (height == h && height > sh && height > WorldGen::WaterLevel + 1)
						pblocks[x][y][z] = blocks::GRASS;
					else if (height<h && height>sh && height > WorldGen::WaterLevel + 1)
						pblocks[x][y][z] = blocks::DIRT;
					else if ((height >= sh - 5 || height >= h - 5) && height <= h && (height <= sh || height <= WorldGen::WaterLevel + 1))
						pblocks[x][y][z] = blocks::SAND;
					else if ((height < sh - 5 && height < h - 5) && height >= 1 && height <= h)
						pblocks[x][y][z] = blocks::ROCK;
					else{
						if (height <= WorldGen::WaterLevel){
							pblocks[x][y][z] = blocks::WATER;
							if (skylight - (WorldGen::WaterLevel - height) * 2 < BRIGHTNESSMIN)
								pbrightness[x][y][z] = BRIGHTNESSMIN;
							else
								pbrightness[x][y][z] = skylight - (brightness)((WorldGen::WaterLevel - height) * 2);
						}
						else
						{
							pblocks[x][y][z] = blocks::AIR;
							pbrightness[x][y][z] = skylight;
						}
					}

					if (pblocks[x][y][z] != blocks::AIR) EmptyChunk = false;
				}

			}
		}
		isEmptyChunk = EmptyChunk;
	}

	void chunk::Load(){
		create();
#ifndef DEBUG_NO_FILEIO
		if (fileExist())
			LoadFromFile();
		else
			build();
#else
		build();
#endif
		updated = true;
		if (cy < worldheight - 1) setChunkUpdated(cx, cy + 1, cz, true);
		if (cy > -worldheight) setChunkUpdated(cx, cy - 1, cz, true);
		if (cx < worldsize - 1) setChunkUpdated(cx + 1, cy, cz, true);
		if (cx > -worldsize) setChunkUpdated(cx - 1, cy, cz, true);
		if (cz < worldsize - 1) setChunkUpdated(cx, cy, cz + 1, true);
		if (cz > -worldsize) setChunkUpdated(cx, cy, cz - 1, true);
	}

	void chunk::Unload(){
		if (this == nullptr) return;
#ifndef DEBUG_NO_FILEIO
		SaveToFile();
#endif
		destroylists();
		destroy();
	}

	string chunk::getFileName() {
		std::stringstream ss;
		ss << "Worlds\\" << worldname << "\\chunks\\chunk_" << cx << "_" << cy << "_" << cz << ".NEWorldChunk";
		return ss.str();
	}

	bool chunk::fileExist(){
		std::fstream file;
		file.open(getFileName(), std::ios::in);
		bool ret = file.is_open();
		file.close();
		return ret;
	}

	void chunk::LoadFromFile(){
		std::ifstream file(getFileName(), std::ios::in, std::ios::binary);
		file.read((char*)pblocks, sizeof(pblocks));
		file.read((char*)pbrightness, sizeof(pbrightness));
		file.close();
		isEmptyChunk = false;
	}

	void chunk::SaveToFile(){
		//if (this == nullptr) return
		if (!isEmptyChunk&&Modified){
			std::ofstream file(getFileName(), std::ios::binary | std::ios::out);
			file.write((char*)pblocks, sizeof(pblocks));
			file.write((char*)pbrightness, sizeof(pbrightness));
			file.close();
		}
	}

	void chunk::buildlists(){
		if (isEmptyChunk) return;
		//建立chunk显示列表
		int x, y, z;
		if (rebuiltChunks >= 2) return;
		rebuiltChunks++;
		updatedChunks++;

		if (list == 0){
			list = glGenLists(3);
			loadAnim = 100.0;
		}
		
		renderer::Init();
		glBindTexture(GL_TEXTURE_2D, BlockTextures);
		for (x = 0; x != 16; x++){
			for (y = 0; y != 16; y++){
				for (z = 0; z != 16; z++){
					if (BlockInfo(pblocks[x][y][z]).isOpaque())
						renderblock(x, y, z, this);
				}
			}
		}
		glNewList(list, GL_COMPILE);
		renderer::Flush();
		glEndList();

		
		renderer::Init();
		glBindTexture(GL_TEXTURE_2D, BlockTextures);
		for (x = 0; x != 16; x++){
			for (y = 0; y != 16; y++){
				for (z = 0; z != 16; z++){
					if (!BlockInfo(pblocks[x][y][z]).isOpaque() && BlockInfo(pblocks[x][y][z]).isSolid())
						renderblock(x, y, z, this);
				}
			}
		}
		glNewList(list + 1, GL_COMPILE);
		renderer::Flush();
		glEndList();

		renderer::Init();
		glBindTexture(GL_TEXTURE_2D, BlockTextures);
		for (x = 0; x != 16; x++){
			for (y = 0; y != 16; y++){
				for (z = 0; z != 16; z++){
					if (!BlockInfo(pblocks[x][y][z]).isSolid() && pblocks[x][y][z] != 0)
						renderblock(x, y, z, this);
				}
			}
		}
		glNewList(list + 2, GL_COMPILE);
		renderer::Flush();
		glEndList();
		updated = false;

	}

	void chunk::destroylists(){

		if (list != 0){

			world::displayListUnloadList.push_back(list);
			list = 0;

		}

	}

	brightness chunk::getbrightness(ubyte x, ubyte y, ubyte z){

		//获取区块内的亮度
		//if (isEmptyChunk) return cy < 0 ? BRIGHTNESSMIN : skylight

		brightness ret = skylight;
		if (pbrightness == nullptr){
			printf("[console][Warning]");
			printf("Chunk(%d,%d,%d)has been unloaded,when getting brightness.\n", cx, cy, cz);
		}
		else
			ret = pbrightness[x][y][z];
		return ret;
	}

	void chunk::setbrightness(ubyte x, ubyte y, ubyte z, brightness tbrightness){
		//设置亮度
		if (this == nullptr) return;
		pbrightness[x][y][z] = tbrightness;
		Modified = true;

	}

	void chunk::putblock(ubyte x, ubyte y, ubyte z, block iblock){

		//这个sub和上面那个是一样的，只是本人的完美主义（说白了就是强迫症）驱使我再写一遍= =
		setblock(x, y, z, iblock);

	}

	void chunk::pickblock(ubyte x, ubyte y, ubyte z){

		//这个sub根本没有存在的必要，其功能等于setblock(x,y,z,0)或putblock(x,y,z,0),但是本人的完...
		setblock(x, y, z, blocks::AIR);

	}

	brightness chunk::getbrightness(int x, int y, int z){
		return getbrightness((ubyte)x, (ubyte)y, (ubyte)z);
	}

	void chunk::setbrightness(int x, int y, int z, brightness tbrightness){
		setbrightness((ubyte)x, (ubyte)y, (ubyte)z, tbrightness);
	}

	void chunk::putblock(int x, int y, int z, block iblock){
		putblock((ubyte)x, (ubyte)y, (ubyte)z, iblock);
	}

	void chunk::pickblock(int x, int y, int z){
		pickblock((ubyte)x, (ubyte)y, (ubyte)z);
	}

	bool chunk::getUpdated(){
		return updated;
	}

	void chunk::setUpdated(bool value){
		updated = value;
	}

	void chunk::callList(int l){
		if (list > 0 && glIsList(list + l)) glCallList(list + l);
	}

	void chunk::hiddenChunkTest(){
		if (isEmptyChunk) return;
		int xp = cx * 16;
		int yp = cy * 16;
		int zp = cz * 16;
		//bottom
		for (int x = 0; x != 16; x++){
			for (int z = 0; z != 16; z++){
				if (getblock(xp + x, yp - 1, zp + z) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}

		//left
		for (int y = 0; y != 16; y++){
			for (int z = 0; z != 16; z++){
				if (getblock(cx - 1, yp + y, zp + z) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}

		//right
		for (int y = 0; y != 16; y++){
			for (int z = 0; z != 16; z++){
				if (getblock(cx + 16, yp + y, zp + z) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}

		//front
		for (int x = 0; x != 16; x++){
			for (int y = 0; y != 16; y++){
				if (getblock(xp + x, yp + y, cz - 1) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}

		//back
		for (int x = 0; x != 16; x++){
			for (int y = 0; y != 16; y++){
				if (getblock(xp + x, yp + y, cz + 16) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}


		//top
		for (int x = 0; x != 16; x++){
			for (int z = 0; z != 16; z++){
				if (getblock(xp + x, yp + 16, zp + z) == blocks::AIR){
					isHiddenChunk = false;
					return;
				}
			}
		}

		isHiddenChunk = true;
	}
}