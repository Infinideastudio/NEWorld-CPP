#pragma once

//Perlin Noise 2D;
namespace WorldGen{

	enum{
		INTERPOLATION_METHOD_LINEAR,       //性能最好的插值算法
		INTERPOLATION_METHOD_COSINE,      //来自国外网站的插值算法
		INTERPOLATION_METHOD_POWER2,      //原创插值算法
		INTERPOLATION_METHOD_POWER3,      //原创插值算法2
		INTERPOLATION_METHOD_JELAWAT,     //Jelawat地鼠 使用的算法
	};

	void perlinNoiseInit(int mapseed);
	int getHeight(int x, int y);
	int getSandHeight(int x, int y);

	extern float perm[256];
	extern int seed;
	extern int NoiseSmoothed;
	extern int InterpolationMethod;
	extern float NoiseScaleX;
	extern float NoiseScaleZ;
	extern int WaterLevel;

	void perlinNoiseInit(int mapseed);

	float Noise(int x, int y);

	float SmoothedNoise(float x, float y);

	float Interpolate(float a, float b, float x);

	float InterpolatedNoise(float x, float y);

	float PerlinNoise2D(float x, float y);

	int getHeight(int x, int y);

}