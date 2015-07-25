#pragma once
#include "Def.h"
typedef unsigned short block;
namespace blocks{  //方块ID;
	enum{
		AIR, ROCK, GRASS, DIRT, STONE, PLANK, WOOD, BEDROCK, LEAF,
		GLASS, WATER, LAVA, GLOWSTONE, SAND, CEMENT, ICE, COAL, IRON,
		EOE
	};

	class SingleBlock{
	private:
		string name;
		bool Solid;
		bool Opaque;
		bool Translucent;
		bool Dark;
	public:
		SingleBlock(string blockName, bool solid, bool opaque, bool translucent, bool dark) :
			name(blockName), Solid(solid), Opaque(opaque), Translucent(translucent), Dark(dark){};

		//获得方块名称
		inline string getBlockName()const{ return name; }
		//是否是固体
		inline bool isSolid()const{ return Solid; }
		//是否不透明
		inline bool isOpaque()const{ return Opaque; }
		//是否半透明
		inline bool isTranslucent()const{ return Translucent; }
		//是否需要平滑光照
		inline bool isDark()const{ return Dark; }
	};

	//class BlockInfo{
	//private:
	//	static const SingleBlock blockData[EOE];
	//	int blockID;
	//public:
	//	BlockInfo(block blockid){
	//		blockID = blockid;
	//		if (blockid >= EOE){
	//			printf("Wrong block ID (%d)!\n", blockid);
	//			blockID = blocks::AIR;
	//		}
	//	};
	//	//获得方块名称
	//	inline const string getBlockName()const{ return blockData[blockID].getBlockName(); }
	//	//是否是固体
	//	inline const bool isSolid()const{ return blockData[blockID].isSolid(); }
	//	//是否不透明
	//	inline const bool isOpaque()const{ return blockData[blockID].isOpaque(); }
	//	//是否半透明
	//	inline const bool isTranslucent()const{ return blockData[blockID].isTranslucent(); }
	//	//是否需要平滑光照
	//	inline const bool isDark()const{ return blockData[blockID].isDark(); }
	//};
	const SingleBlock blockData[EOE] = {
			//		    方块名称		  固体	 不透明	  半透明	 暗(dark)
			SingleBlock("Air"		, false	, false	, false	, false	),
			SingleBlock("Rock"		, true	, true	, false	, true	),
			SingleBlock("Grass"		, true	, true	, false	, true	),
			SingleBlock("Dirt"		, true	, true	, false	, true	),
			SingleBlock("Stone"		, true	, true	, false	, true	),
			SingleBlock("Plank"		, true	, true	, false	, true	),
			SingleBlock("Wood"		, true	, true	, false	, true	),
			SingleBlock("Bedrock"	, true	, true	, false	, true	),
			SingleBlock("Leaf"		, true	, false	, false	, false	),
			SingleBlock("Glass"		, true	, false	, false	, false	),
			SingleBlock("Water"		, false	, false	, true	, false	),
			SingleBlock("Lava"		, false	, false	, true	, false	),
			SingleBlock("GlowStone"	, true	, true	, false	, false	),
			SingleBlock("Sand"		, true	, true	, false	, true	),
			SingleBlock("Ice"		, true	, false	, true	, false	),
			SingleBlock("cement"	, true	, true	, false	, true	),
			SingleBlock("Coal Block", true	, true	, false	, true	),
			SingleBlock("Iron Block", true	, true	, false	, true	)
		};
	inline const SingleBlock& BlockInfo(block blockID){
		return blockData[blockID];
	}
}

using blocks::BlockInfo;
