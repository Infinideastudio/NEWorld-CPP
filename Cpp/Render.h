#pragma once
#include "Def.h"
#include "World.h"

extern int renderedChunk;

extern int selx, sely, selz, oldselx, oldsely, oldselz;
extern float selt, seldes;
extern block selb;
extern brightness selbr;
extern int selbx, selby, selbz, selcx, selcy, selcz;

extern int fps;                               //帧速率
extern int fpsc;                              //帧数计数
extern double fctime;
extern int ups;
extern int upsc;
extern double uctime;

extern bool GUIrenderswitch;
extern bool DEBUGMODE;                  //调试模式
extern bool DebugHitbox;            //Hitbox调试模式
extern bool DebugChunk;             //区块调试模式[Not enabled!]
extern bool gamebegin;
extern bool loading;
extern bool bagOpened;


extern bool shouldGetScreenshot, shouldGetThumbnail;

extern TextureID splTex;
extern TextureID BlockTexture[20];
extern TextureID BlockTextures;
extern TextureID guiImage[6];
extern TextureID DestroyImage[11];
extern TextureID Skins[2];


void splashscreen();
void drawcloud(double px, double pz);
void drawLoading();
void drawMain();
void drawBag();
void drawGUI();
void drawborder(int x, int y, int z);
void renderDestroy(float level, int x, int y, int z);