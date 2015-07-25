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
		//清空&初始化~
		firstCall = false;
		if (VertexArray.capacity() == 0){
			VertexArray.reserve(ArrayUNITSIZE * 3);
			TexcoordArray.reserve(ArrayUNITSIZE * 2);
			ColorArray.reserve(ArrayUNITSIZE * 3);
		}

		VertexArray.clear();
		TexcoordArray.clear();
		ColorArray.clear();
		Vertexes = 0;
		Textured = false;
		Colored = false;
	}

	void Vertex3d(double x, double y, double z){

		if (Textured){
			TexcoordArray.push_back(texcoordX);
			TexcoordArray.push_back(texcoordY);
		}

		if (Colored){
			ColorArray.push_back(colorR);
			ColorArray.push_back(colorG);
			ColorArray.push_back(colorB);
		}

		Vertexes++;
		VertexArray.push_back(x);
		VertexArray.push_back(y);
		VertexArray.push_back(z);

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
				//glTexcoordPointer(2,GL_DOUBLE,0,TexcoordArray);
			}

			if (Colored){
				glBindBuffer/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Buffers[2]);
				glBufferData/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Vertexes * 3 * sizeof(double), &ColorArray[0], GL_STATIC_DRAW);
				glColorPointer(3, GL_DOUBLE, 0, 0);
				//glColorPointer(3,GL_DOUBLE,0,ColorArray);
			}

			glBindBuffer/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Buffers[0]);
			glBufferData/*ARB*/(GL_ARRAY_BUFFER/*_ARB*/, Vertexes * 3 * sizeof(double), &VertexArray[0], GL_STATIC_DRAW);
			glVertexPointer(3, GL_DOUBLE, 0, 0);
			//glVertexPointer(3,GL_DOUBLE,0,VertexArray);

			//================================//;
			glDrawArrays(GL_QUADS, 0, Vertexes);
			//================================//;

			//glDeleteBuffers/*ARB*/(3, Buffers);

			//重置;
			Init();

		}
	}
}