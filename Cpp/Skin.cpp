#include "Skin.h"
#include "Textures.h"

void Skin::renderer(){
	if (texture == -1) return;
}

void Skin::loadSkin(string path, string pathMask, int type){
	texture = textures::LoadRGBATexture(path, pathMask);
	skinType = type;
}