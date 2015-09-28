#pragma once
#include "Def.h"
#include "World.h"

namespace world{
	uint64 getChunkID(int x, int y, int z);
	class chunkHeightMap {
	public:
		chunkHeightMap(uint64 chid) : cx(0), cz(0), id(chid) {};
		chunkHeightMap(int x, int z) : cx(x), cz(z), id(getChunkID(x, 0, z)) {};

		bool operator== (const chunkHeightMap& hm) const
		{
			return id == hm.id;
		}

		bool operator== (const uint64& hmid) const
		{
			return id == hmid;
		}

		bool operator< (const chunkHeightMap& hm) const
		{
			return id < hm.id;
		}

		bool operator< (const uint64& hmid) const
		{
			return id < hmid;
		}

		int terrain[16][16];

		void build();

	private:
		const int cx, cz;
		const uint64 id;
	};
}