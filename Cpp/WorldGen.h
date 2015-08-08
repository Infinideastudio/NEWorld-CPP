#pragma once

//Perlin Noise 2D;
namespace WorldGen{

	enum{
		INTERPOLATION_METHOD_LINEAR,      //性能最好的插值算法
		INTERPOLATION_METHOD_COSINE,      //来自国外网站的插值算法
		INTERPOLATION_METHOD_POWER2,      //原创插值算法
		INTERPOLATION_METHOD_POWER3,      //原创插值算法2
		INTERPOLATION_METHOD_JELAWAT,     //Jelawat地鼠 使用的算法
	};

	extern int seed;
	extern int WaterLevel;
	void perlinNoiseInit(int mapseed);
	
	int getHeight(int x, int y);

	int getSandHeight(int x, int y);

	void perlinNoiseInit(int mapseed);

	double Noise(int x, int y);

	double SmoothedNoise(double x, double y);

	double Interpolate(double a, double b, double x);

	double InterpolatedNoise(double x, double y);

	double PerlinNoise2D(double x, double y);

	int getHeight(int x, int y);

}