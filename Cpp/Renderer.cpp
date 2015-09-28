#include "renderer.h"
namespace renderer {
	bool Textured, Colored;
	int Vertexes;
	vector<float> VertexArray;
	float texcoordX, texcoordY;
	float colorR, colorG, colorB;
	uint Buffers[3];
	bool ShaderAval, EnableShaders = false;
	GLhandleARB shaders[16];
	GLhandleARB shaderPrograms[16];
	int shadercount = 0;
	int index = 0;

	void Init() {
		static bool firstCall = true;
		if (Vertexes == 0 && !firstCall) return; //Vertexes为0时，判断其已经初始化，无需再次初始化
		if (firstCall) {
			VertexArray.resize(ArrayUNITSIZE * 3);
			firstCall = false;
		}

		index = 0;
		Vertexes = 0;
		Textured = false;
		Colored = false;
	}

	void Vertex3d(double x, double y, double z)
	{
		Vertex3d((float)x, (float)y, (float)z);
	}

	void TexCoord2d(double x, double y)
	{
		TexCoord2d((float)x, (float)y);
	}

	void Color3d(double r, double g, double b)
	{
		Color3d((float)r, (float)g, (float)b);
	}

	void Vertex3d(float x, float y, float z) {
		if (Textured) {
			VertexArray[index++] = texcoordX;
			VertexArray[index++] = texcoordY;
		}

		if (Colored) {
			VertexArray[index++] = colorR;
			VertexArray[index++] = colorG;
			VertexArray[index++] = colorB;
		}

		VertexArray[index++] = x;
		VertexArray[index++] = y;
		VertexArray[index++] = z;

		Vertexes++;
	}

	void TexCoord2d(float x, float y) {

		texcoordX = x;
		texcoordY = y;
		Textured = true;

	}

	void Color3d(float r, float g, float b) {

		colorR = r;
		colorG = g;
		colorB = b;
		Colored = true;

	}

	void Flush(uint& buffer, int& vtxs) {

		//上次才知道原来Flush还有冲厕所的意思QAQ
		//OpenGL有个函数glFlush()，翻译过来就是GL冲厕所() ←_←

		vtxs = Vertexes;

		if (EnableShaders) {
			for (int i = 0; i != shadercount; i++) {
				glUseProgramObjectARB(shaderPrograms[i]);
			}
		}

		if (Vertexes > 0) {

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffer);

			if (Textured) {
				if (Colored)
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, Vertexes * 8 * sizeof(float), VertexArray.data(), GL_STATIC_DRAW);
				else
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, Vertexes * 5 * sizeof(float), VertexArray.data(), GL_STATIC_DRAW);
			}
			else {
				if (Colored)
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, Vertexes * 6 * sizeof(float), VertexArray.data(), GL_STATIC_DRAW);
				else
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, Vertexes * 3 * sizeof(float), VertexArray.data(), GL_STATIC_DRAW);
			}

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			//重置
			Init();

		}

		if (EnableShaders) glUseProgramObjectARB(0);

	}
    
	void renderbuffer(uint buffer, int vtxs, bool ftex, bool fcol) {

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffer);
		if (ftex) {
			if (fcol) {
				glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), (float*)0);
				glColorPointer(3, GL_FLOAT, 8 * sizeof(float), (float*)(2 * sizeof(float)));
				glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), (float*)(5 * sizeof(float)));
			}
			else {
				glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (float*)0);
				glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (float*)(2 * sizeof(float)));
			}
		}
		else {
			if (fcol) {
				glColorPointer(3, GL_FLOAT, 6 * sizeof(float), (float*)0);
				glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), (float*)(3 * sizeof(float)));
			}
			else {
				glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (float*)0);
			}
		}

		//================================
		glDrawArrays(GL_QUADS, 0, vtxs);
		//================================

	}
    
	//GLhandleARB loadShader(string filename, uint mode){
        
		//GLhandleARB shader;
  //      dim as zstring ptr ptr shaderSource
  //      dim as integer ptr shaderSourceLen
  //      dim as integer shaderLineNums
  //      dim shaderSourceCurLine as zstring ptr
  //      dim shaderSourceCurLineStr as string
  //      dim f as integer
  //      f=freefile
  //      open filename for input as #f
  //      do until eof(f)
  //          line input #f,shaderSourceCurLineStr
  //          print shaderSourceCurLineStr
  //          shaderSourceCurLine=allocate(len(shaderSourceCurLineStr)+2)
  //          *shaderSourceCurLine=shaderSourceCurLineStr+chr(10)
  //          shaderlinenums+=1
  //          shaderSource=reallocate(shaderSource,shaderlinenums*sizeof(zstring ptr))
  //          shaderSource[shaderlinenums-1]=shaderSourceCurLine
  //          print *shaderSource[shaderlinenums-1]
  //          shaderSourceLen=reallocate(shaderSourceLen,shaderlinenums*sizeof(integer))
  //          shaderSourceLen[shaderlinenums-1]=len(shaderSourceCurLineStr)+1
  //      loop
  //      close #f
  //      shader=glCreateShaderObjectARB(mode)
  //      glShaderSourceARB(shader,shaderLineNums,shaderSource,0)
  //      glCompileShaderARB(shader)
  //      return shader;
        
    //}
    
    void initShader(){

		for (int i = 0; i != shadercount; i++) {
			shaderPrograms[i] = glCreateProgramObjectARB();
			glAttachObjectARB(shaderPrograms[i*2], shaders[i]);
			glAttachObjectARB(shaderPrograms[i*2+1], shaders[i]);
			glLinkProgramARB(shaderPrograms[i]);
		}

    }
}