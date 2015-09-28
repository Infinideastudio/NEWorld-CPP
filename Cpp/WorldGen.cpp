#include "WorldGen.h"
#include "Def.h"
//Perlin Noise 2D
namespace WorldGen{
	double	perm[256];
	int		seed;
	int		NoiseSmoothed = 1;
	double	NoiseScaleX = 32;
	double	NoiseScaleZ = 32;
	int		WaterLevel = 30;

	void perlinNoiseInit(int mapseed){
		srand(mapseed);
		int i;
		for (i = 0; i != 256; i++){
			perm[i] = (double)(rnd() * 256);
		}
		seed = mapseed;
	}

	inline double Noise(int x, int y){
		return perm[abs(x * 437 + y) && 0xff];
	}

	double SmoothedNoise(double x, double y){
		double corners, sides, center;
		int intx = GoInt(x);
		int inty = GoInt(y);
		corners = (Noise(intx - 1, inty - 1) + Noise(intx + 1, inty - 1) + Noise(intx - 1, inty + 1) + Noise(intx + 1, inty + 1)) / 32;
		sides = (Noise(intx - 1, inty) + Noise(intx + 1, inty) + Noise(intx, inty - 1) + Noise(intx, inty + 1)) / 16;
		center = Noise(intx, inty) / 8;
		return corners + sides + center;
	}

	double Interpolate(double a, double b, double x){
		return a*(1 - x) + b*x;
	}

	double InterpolatedNoise(double x, double y){
		int int_X, int_Y;
		double fractional_X, fractional_Y, v1, v2, v3, v4, i1, i2;
		int_X = GoInt(x);
		fractional_X = x - int_X;
		int_Y = GoInt(y);
		fractional_Y = y - int_Y;
		v1 = SmoothedNoise((double)int_X, (double)int_Y);
		v2 = SmoothedNoise((double)int_X + 1, (double)int_Y);
		v3 = SmoothedNoise((double)int_X, (double)int_Y + 1);
		v4 = SmoothedNoise((double)int_X + 1, (double)int_Y + 1);
		i1 = Interpolate(v1, v2, fractional_X);
		i2 = Interpolate(v3, v4, fractional_X);
		return Interpolate(i1, i2, fractional_Y);
	}

	double PerlinNoise2D(double x, double y){
		double total = 0, frequency = 1, amplitude = 1;
		for (int i = 0; i <= 7; i++){
			total += InterpolatedNoise(x*frequency, y*frequency)*amplitude;
			frequency *= 2;
			amplitude /= 2;
		}
		return total;
	}

	int getHeight(int x, int y){
		double NoiseHeight = PerlinNoise2D(x / NoiseScaleX, y / NoiseScaleZ) / 4.0f;
		return int(NoiseHeight);
	}
}