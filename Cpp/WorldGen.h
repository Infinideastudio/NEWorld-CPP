#pragma once

//Perlin Noise 2D;
namespace WorldGen{

	enum{
		INTERPOLATION_METHOD_LINEAR,      //������õĲ�ֵ�㷨
		INTERPOLATION_METHOD_COSINE,      //���Թ�����վ�Ĳ�ֵ�㷨
		INTERPOLATION_METHOD_POWER2,      //ԭ����ֵ�㷨
		INTERPOLATION_METHOD_POWER3,      //ԭ����ֵ�㷨2
		INTERPOLATION_METHOD_JELAWAT,     //Jelawat���� ʹ�õ��㷨
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