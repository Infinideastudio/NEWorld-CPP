#include "Textures.h"
#include <fstream>

int BLOCKTEXTURE_SIZE, BLOCKTEXTURE_UNITSIZE, filter;

namespace textures{

	void Init(){
		std::ifstream options("textures\\Blocks\\options.txt", std::ios::in); //���ļ�
		options >> BLOCKTEXTURE_SIZE;
		options >> BLOCKTEXTURE_UNITSIZE;
		options >> filter;
		options.close();
	}

	ubyte getTextureIndex(block blockname, ubyte side){
		switch (blockname){
		case blocks::AIR:
			return AIR;
		case blocks::ROCK:
			return ROCK;
		case blocks::GRASS:
			switch (side){
			case 1:
				return GRASS_TOP;
			case 2:
				return GRASS_SIDE;
			case 3:
				return DIRT;
			}
		case blocks::DIRT:
			return DIRT;
		case blocks::STONE:
			return STONE;
		case blocks::PLANK:
			return PLANK;
		case blocks::WOOD:
			switch (side){
			case 1:
				return WOOD_TOP;
			case 2:
				return WOOD_SIDE;
			case 3:
				return WOOD_TOP;
			}
		case blocks::BEDROCK:
			return BEDROCK;
		case blocks::LEAF:
			return LEAF;
		case blocks::GLASS:
			return GLASS;
		case blocks::WATER:
			return WATER;
		case blocks::LAVA:
			return LAVA;
		case blocks::GLOWSTONE:
			return GLOWSTONE;
		case blocks::SAND:
			return SAND;
		case blocks::CEMENT:
			return CEMENT;
		case blocks::ICE:
			return ICE;
		case blocks::COAL:
			return COAL;
		case blocks::IRON:
			return IRON;
		default:
			return UNKNOWN;
		}
	}

	double getTexcoordX(block iblock, ubyte side){
		return ((getTextureIndex(iblock, side) - 1) % (BLOCKTEXTURE_SIZE / BLOCKTEXTURE_UNITSIZE))*(BLOCKTEXTURE_UNITSIZE / (double)BLOCKTEXTURE_SIZE);
	}

	double getTexcoordY(block iblock, ubyte side){
		return (int((getTextureIndex(iblock, side) - 1) / (BLOCKTEXTURE_SIZE / (double)BLOCKTEXTURE_UNITSIZE)))*(BLOCKTEXTURE_UNITSIZE / (double)BLOCKTEXTURE_SIZE);
	}

	TEXTURE_RGB LoadRGBTexture(string Filename){
		unsigned char col[3];
		unsigned int ind = 0;
		TEXTURE_RGB bitmap; //����λͼ
		bitmap.buffer = nullptr; bitmap.sizeX = bitmap.sizeY = 0;
		std::ifstream bmpfile(Filename, std::ios::binary | std::ios::in); //λͼ�ļ��������ƣ�
		if (!bmpfile.is_open()){
			printf("[console][Warning] Cannot load &s\n", Filename.c_str());
			return TEXTURE_RGB();
		}
		BITMAPINFOHEADER bih; //���ֹ���λͼ�Ĳ���
		BITMAPFILEHEADER bfh; //���ֹ����ļ��Ĳ���
		//��ʼ��ȡ
		bmpfile.read((char*)&bfh, sizeof(BITMAPFILEHEADER));
		bmpfile.read((char*)&bih, sizeof(BITMAPINFOHEADER));
		bitmap.sizeX = bih.biWidth;
		bitmap.sizeY = bih.biHeight;
		bitmap.buffer = shared_ptr<ubyte>(new unsigned char[bitmap.sizeX * bitmap.sizeY * 3]);
		for (unsigned int i = 0; i < bitmap.sizeX * bitmap.sizeY; i++){
			//��BGR��ʽת��ΪRGB��ʽ
			bmpfile.read((char*)col, 3);
			bitmap.buffer.get()[ind++] = col[2]; //R
			bitmap.buffer.get()[ind++] = col[1]; //G
			bitmap.buffer.get()[ind++] = col[0]; //B
		}
		bmpfile.close();
		return bitmap;
	}

	TEXTURE_RGBA LoadRGBATexture(string Filename, string MkFilename){
		unsigned char col[3];
		unsigned int ind = 0;
		TEXTURE_RGBA bitmap; //����λͼ
		bitmap.buffer = nullptr; bitmap.sizeX = bitmap.sizeY = 0;
		std::ifstream bmpfile(Filename, std::ios::binary | std::ios::in); //λͼ�ļ��������ƣ�
		std::ifstream maskfile(MkFilename, std::ios::binary | std::ios::in); //����λͼ�ļ��������ƣ�
		if (!bmpfile.is_open()){
			printf("[console][Warning] Cannot load &s\n", Filename.c_str());
			return TEXTURE_RGBA();
		}
		BITMAPFILEHEADER bfh; //���ֹ����ļ��Ĳ���
		BITMAPINFOHEADER bih; //���ֹ���λͼ�Ĳ���
		//��ʼ��ȡ
		maskfile.read((char*)&bfh, sizeof(BITMAPFILEHEADER)); //��������ռλmask�ļ���
		maskfile.read((char*)&bih, sizeof(BITMAPINFOHEADER)); //���˺���mask����ֱ�Ӵ���ɫ���ֿ�ʼ��ȡ
		bmpfile.read((char*)&bfh, sizeof(BITMAPFILEHEADER)); //������info�����bmp�ļ�Ϊ׼
		bmpfile.read((char*)&bih, sizeof(BITMAPINFOHEADER)); //��������֮ǰ��mask�ļ���������info����
		bitmap.sizeX = bih.biWidth;
		bitmap.sizeY = bih.biHeight;
		bitmap.buffer = shared_ptr<ubyte>(new unsigned char[bitmap.sizeX * bitmap.sizeY * 4]);
		for (unsigned int i = 0; i < bitmap.sizeX * bitmap.sizeY; i++){
			//��BGR��ʽת��ΪRGB��ʽ
			bmpfile.read((char*)col, 3);
			bitmap.buffer.get()[ind++] = col[2]; //R
			bitmap.buffer.get()[ind++] = col[1]; //G
			bitmap.buffer.get()[ind++] = col[0]; //B
			if (MkFilename == ""){
				bitmap.buffer.get()[ind++] = 255;
			}
			else{
				//������ͼ�ĺ�ɫͨ��������ΪAlphaͨ��
				maskfile.read((char*)col, 3);
				bitmap.buffer.get()[ind++] = 255 - col[2]; //A
			}
		}
		bmpfile.close();
		maskfile.close();
		return bitmap;
	}

	TEXTURE_RGBA LoadFontTexture(string Filename){
		TEXTURE_RGBA Texture;
		TEXTURE_RGB image;
		ubyte *ip, *tp;
		image = LoadRGBTexture(Filename);
		Texture.sizeX = image.sizeX;
		Texture.sizeY = image.sizeY;
		Texture.buffer = shared_ptr<ubyte>(new unsigned char[image.sizeX * image.sizeY * 4]);
		if (Texture.buffer == nullptr){
			printf("[console][Warning] Cannot alloc memory when loading &s\n", Filename.c_str());
			return TEXTURE_RGBA();
		}
		ip = image.buffer.get();
		tp = Texture.buffer.get();
		for (unsigned i = 0; i != image.sizeX*image.sizeY ; i++){
			*tp = 255; tp++;
			*tp = 255; tp++;
			*tp = 255; tp++;
			*tp = 255 - *ip; tp++; ip += 3;
		}
		return Texture;
	}

	void SaveRGBTexture(string filename, TEXTURE_RGB image){
		BITMAPFILEHEADER bitmapfileheader;
		BITMAPINFOHEADER bitmapinfoheader;
		bitmapfileheader.bfType = BITMAP_ID;
		bitmapfileheader.bfSize = image.sizeX*image.sizeY * 3 + 54;
		bitmapfileheader.bfReserved1 = 0;
		bitmapfileheader.bfReserved2 = 0;
		bitmapfileheader.bfOffBits = 54;
		bitmapinfoheader.biSize = 40;
		bitmapinfoheader.biWidth = image.sizeX;
		bitmapinfoheader.biHeight = image.sizeY;
		bitmapinfoheader.biPlanes = 1;
		bitmapinfoheader.biBitCount = 24;
		bitmapinfoheader.biCompression = 0;
		bitmapinfoheader.biSizeImage = image.sizeX*image.sizeY * 3;
		bitmapinfoheader.biXPelsPerMeter = 0;
		bitmapinfoheader.biYPelsPerMeter = 0;
		bitmapinfoheader.biClrUsed = 0;
		bitmapinfoheader.biClrImportant = 0;
		ubyte* p;
		ubyte r, g, b;
		std::ofstream ofs(filename, std::ios::in | std::ios::binary);
		ofs.write((char*)&bitmapfileheader, sizeof(bitmapfileheader));
		ofs.write((char*)&bitmapinfoheader, sizeof(bitmapinfoheader));
		p = image.buffer.get();
		for (unsigned int index = 0; index != image.sizeX*image.sizeY; index++){
			r = *p; p += 1;
			g = *p; p += 1;
			b = *p; p += 1;
			ofs.write((char*)&b, sizeof(ubyte));
			ofs.write((char*)&g, sizeof(ubyte));
			ofs.write((char*)&r, sizeof(ubyte));
		}
		ofs.close();
	}

}