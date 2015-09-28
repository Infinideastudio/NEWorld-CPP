#pragma once

//Perlin Noise 2D
namespace WorldGen{

	extern int seed;
	extern int WaterLevel;
	void perlinNoiseInit(int mapseed);

	void perlinNoiseInit(int mapseed);

	double Noise(int x, int y);

	double SmoothedNoise(double x, double y);

	double Interpolate(double a, double b, double x);

	double InterpolatedNoise(double x, double y);

	double PerlinNoise2D(double x, double y);

	int getHeight(int x, int y);

}