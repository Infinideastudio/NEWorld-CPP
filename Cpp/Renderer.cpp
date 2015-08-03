#include "renderer.h"
namespace renderer{
	bool Textured, Colored;
	int Vertexes;
	vector<double> VertexArray, TexcoordArray, ColorArray;
	double texcoordX, texcoordY;
	double colorR, colorG, colorB;
	uint Buffers[3];
	void Init(){
		static bool firstCall = true;
		if (Vertexes == 0 && !firstCall) return; //Vertexes为0时，判断其已经初始化，无需再次初始化
		if (firstCall){
			VertexArray.resize(ArrayUNITSIZE * 3);
			TexcoordArray.resize(ArrayUNITSIZE * 2);
			ColorArray.resize(ArrayUNITSIZE * 3);
			firstCall = false;
		}

		Vertexes = 0;
		Textured = false;
		Colored = false;
	}

	void Vertex3d(double x, double y, double z){
		if (Textured) {
			TexcoordArray[Vertexes * 2] = texcoordX;
			TexcoordArray[Vertexes * 2 + 1] = texcoordY;
		}

		if (Colored) {
			ColorArray[Vertexes * 3] = colorR;
			ColorArray[Vertexes * 3 + 1] = colorG;
			ColorArray[Vertexes * 3 + 2] = colorB;
		}

		VertexArray[Vertexes * 3] = x;
		VertexArray[Vertexes * 3 + 1] = y;
		VertexArray[Vertexes * 3 + 2] = z;

		Vertexes++;
	}

	void TexCoord2d(double x, double y){

		texcoordX = x;
		texcoordY = y;
		Textured = true;

	}

	void Color3d(double r, double g, double b){

		colorR = r;
		colorG = g;
		colorB = b;
		Colored = true;

	}

	void Flush(){

		//上次才知道原来Flush还有冲厕所的意思QAQ;
		//OpenGL有个函数glFlush()，翻译过来就是GL冲厕所() ←_←;

		if (Vertexes > 0){
			memset(Buffers, 0, sizeof(Buffers));
			glGenBuffers/*ARB*/(3, Buffers);

			if (Textured){
				glBindBuffer/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Buffers[1]);
				glBufferData/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Vertexes * 2 * sizeof(double), &TexcoordArray[0], GL_STATIC_DRAW);
				glTexCoordPointer(2, GL_DOUBLE, 0, 0);
			}

			if (Colored){
				glBindBuffer/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Buffers[2]);
				glBufferData/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Vertexes * 3 * sizeof(double), &ColorArray[0], GL_STATIC_DRAW);
				glColorPointer(3, GL_DOUBLE, 0, 0);
			}

			glBindBuffer/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Buffers[0]);
			glBufferData/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Vertexes * 3 * sizeof(double), &VertexArray[0], GL_STATIC_DRAW);
			glVertexPointer(3, GL_DOUBLE, 0, 0);

			//================================//;
			glDrawArrays(GL_QUADS, 0, Vertexes);
			//================================//;

			glDeleteBuffers/*ARB*/(3, Buffers);

			//重置;
			Init();

		}
	}
}