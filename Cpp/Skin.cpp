#include "Skin.h"
#include "Textures.h"
#include <map>
std::map<string, TextureID> loadedSkins;

void Skin::renderer(){
	return; //ÓÐBUG

	if (texture == 0) return;
	glDisable(GL_CULL_FACE);
	glTranslatef(X, Y, Z);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	if (skinType == CATSKIN){
		//===Head===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 8); glVertex3d(-0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 8);  glVertex3d(-0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 8);  glVertex3d(-0.141, 0.141, 0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 8);  glVertex3d(-0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 6);  glVertex3d(-0.141, -0.375, -0.141);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 6);  glVertex3d(-0.141, -0.375, 0.141);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 8);  glVertex3d(0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 8);  glVertex3d(0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 4);  glVertex3d(0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 4);  glVertex3d(0.141, 0.141, 0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 2);  glVertex3d(0.141, -0.375, 0.141);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 2);  glVertex3d(0.141, -0.375, -0.141);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 8);  glVertex3d(0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 8);  glVertex3d(-0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 6);  glVertex3d(0.141, 0.141, 0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 6);  glVertex3d(-0.141, 0.141, 0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 4);  glVertex3d(-0.141, -0.375, 0.141);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 4);  glVertex3d(0.141, -0.375, 0.141);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 8);  glVertex3d(-0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 8);  glVertex3d(0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 2);  glVertex3d(-0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 2);  glVertex3d(0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 0);  glVertex3d(0.141, -0.375, -0.141);
		glTexCoord2d(1.0 / 8 * 8, 1.0 / 8 * 0);  glVertex3d(-0.141, -0.375, -0.141);
		//Top
		glNormal3d(0.0, 1.0, 0.0);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 8);  glVertex3d(0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 8);  glVertex3d(-0.125, 0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 7);  glVertex3d(-0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 7);  glVertex3d(0.125, 0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 8);  glVertex3d(0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 8);  glVertex3d(-0.141, 0.141, -0.141);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 7);  glVertex3d(-0.141, 0.141, 0.141);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 7);  glVertex3d(0.141, 0.141, 0.141);
		//Bottom
		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 8);  glVertex3d(-0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 8);  glVertex3d(0.125, -0.125, -0.125);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.125);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.125);

		//===Body===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);

		//===Left leg===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 2);  glVertex3d(-0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 2);  glVertex3d(-0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 5);  glVertex3d(-0.141, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 5);  glVertex3d(-0.141, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 2);  glVertex3d(-0.141, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 2);  glVertex3d(-0.141, -1.391, 0.0785);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 2);  glVertex3d(0, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 2);  glVertex3d(0, -1.375, -0.0625);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 2);  glVertex3d(-0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 2);  glVertex3d(0, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 5);  glVertex3d(-0.141, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 2);  glVertex3d(-0.141, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 2);  glVertex3d(0, -1.391, 0.0785);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 2);  glVertex3d(0, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 2);  glVertex3d(-0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 5);  glVertex3d(-0.141, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 2);  glVertex3d(0, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 2);  glVertex3d(-0.141, -1.391, -0.0785);
		//Bottom
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1);  glVertex3d(-0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 1);  glVertex3d(0, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 0.5);  glVertex3d(0, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 0.5);  glVertex3d(-0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 1);  glVertex3d(-0.141, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1);  glVertex3d(0, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 0.5);  glVertex3d(0, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 0.5);  glVertex3d(-0.141, -1.391, 0.0785);

		//===Right leg===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 2);  glVertex3d(0, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 2);  glVertex3d(0, -1.375, 0.0625);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 2);  glVertex3d(0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 2);  glVertex3d(0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 5);  glVertex3d(0.141, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 5);  glVertex3d(0.141, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 2);  glVertex3d(0.141, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 2);  glVertex3d(0.141, -1.391, -0.0785);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 2);  glVertex3d(0, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 2);  glVertex3d(0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 5);  glVertex3d(0.141, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 5);  glVertex3d(0, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 2);  glVertex3d(0, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 2);  glVertex3d(0.141, -1.391, 0.0785);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 2);  glVertex3d(0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 2);  glVertex3d(0, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 5);  glVertex3d(0, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 5);  glVertex3d(0.141, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 2);  glVertex3d(0.141, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 2);  glVertex3d(0, -1.391, -0.0785);
		//Bottom
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 0.5);  glVertex3d(0.125, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 0.5);  glVertex3d(0, -1.375, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 0);  glVertex3d(0, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 0);  glVertex3d(0.125, -1.375, -0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 0.5);  glVertex3d(0.141, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 0.5);  glVertex3d(0, -1.391, 0.0785);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1);  glVertex3d(0, -1.391, -0.0785);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 1);  glVertex3d(0.141, -1.391, -0.0785);

		//===Left arm===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 7);  glVertex3d(-0.25, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 7);  glVertex3d(-0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 5);  glVertex3d(-0.25, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 5);  glVertex3d(-0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 2);  glVertex3d(-0.266, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 2);  glVertex3d(-0.266, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 0, 1.0 / 8 * 0);  glVertex3d(-0.266, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 0);  glVertex3d(-0.266, -0.625, 0.0785);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 7);  glVertex3d(-0.25, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 5);  glVertex3d(-0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 2);  glVertex3d(-0.125, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 2);  glVertex3d(-0.266, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 0.5, 1.0 / 8 * 0);  glVertex3d(-0.266, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 0);  glVertex3d(-0.125, -0.625, 0.0785);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 7);  glVertex3d(-0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 7);  glVertex3d(-0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 4.5, 1.0 / 8 * 5);  glVertex3d(-0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 5);  glVertex3d(-0.25, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 2);  glVertex3d(-0.266, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 2);  glVertex3d(-0.125, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 1, 1.0 / 8 * 0);  glVertex3d(-0.125, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 0);  glVertex3d(-0.266, -0.625, -0.0785);
		//Top
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 2);  glVertex3d(-0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 2);  glVertex3d(-0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 1.5);  glVertex3d(-0.25, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1.5);  glVertex3d(-0.125, -0.125, 0.0625);
		//Bottom
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 2);  glVertex3d(-0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 2);  glVertex3d(-0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1.5);  glVertex3d(-0.25, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 1.5);  glVertex3d(-0.125, -0.625, -0.0625);

		//===Right arm===
		//Left
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		//Right
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 7);  glVertex3d(0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 7);  glVertex3d(0.25, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 5.5, 1.0 / 8 * 5);  glVertex3d(0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 5);  glVertex3d(0.25, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 2);  glVertex3d(0.266, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 2);  glVertex3d(0.266, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 1.5, 1.0 / 8 * 0);  glVertex3d(0.266, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 0);  glVertex3d(0.266, -0.625, -0.0785);
		//Front
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 7);  glVertex3d(0.25, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 6, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 5);  glVertex3d(0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 2);  glVertex3d(0.266, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 2);  glVertex3d(0.125, -0.125, 0.0785);
		glTexCoord2d(1.0 / 8 * 2, 1.0 / 8 * 0);  glVertex3d(0.125, -0.625, 0.0785);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 0);  glVertex3d(0.266, -0.625, 0.0785);
		//Back
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 7);  glVertex3d(0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 7);  glVertex3d(0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 6.5, 1.0 / 8 * 5);  glVertex3d(0.25, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 7, 1.0 / 8 * 5);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 2);  glVertex3d(0.125, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 2);  glVertex3d(0.266, -0.125, -0.0785);
		glTexCoord2d(1.0 / 8 * 2.5, 1.0 / 8 * 0);  glVertex3d(0.266, -0.625, -0.0785);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 0);  glVertex3d(0.125, -0.625, -0.0785);
		//Top
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1.5);  glVertex3d(0.25, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 1.5);  glVertex3d(0.125, -0.125, -0.0625);
		glTexCoord2d(1.0 / 8 * 3, 1.0 / 8 * 1);  glVertex3d(0.125, -0.125, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1);  glVertex3d(0.25, -0.125, 0.0625);
		//Bottom
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 1.5);  glVertex3d(0.25, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1.5);  glVertex3d(0.125, -0.625, 0.0625);
		glTexCoord2d(1.0 / 8 * 3.5, 1.0 / 8 * 1);  glVertex3d(0.125, -0.625, -0.0625);
		glTexCoord2d(1.0 / 8 * 4, 1.0 / 8 * 1);  glVertex3d(0.25, -0.625, -0.0625);

		glEnd();
	}
	glTranslatef(-X, -Y, -Z);
	glEnable(GL_CULL_FACE);
}

void Skin::loadSkin(string path, string pathMask, int type){
	auto iter = loadedSkins.find(path);
	if (iter != loadedSkins.end()){
		texture = iter->second;
	}
	else{
		texture = textures::LoadRGBATexture(path, pathMask);
		loadedSkins[path] = texture;
	}
	skinType = type;
}