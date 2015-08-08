#include "WorldGen.h"
#include "Def.h"
//Perlin Noise 2D;
namespace WorldGen{
	double	perm[256];
	int		seed;
	int		NoiseSmoothed = 1;
	int		InterpolationMethod = INTERPOLATION_METHOD_POWER2;
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
		return perm[abs(x * 437 + y) % 256];
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
		double temp, x2;
		switch (InterpolationMethod){
		case INTERPOLATION_METHOD_LINEAR:
			return a*(1 - x) + b*x;
		case INTERPOLATION_METHOD_COSINE:
			return double(a*(1 - (1 - cos(x*M_PI))*0.5) + b*(1 - cos(x*M_PI))*0.5);	
		case INTERPOLATION_METHOD_POWER2:
			temp = 1 - x;
			temp *= temp;
			x2 = x*x;
			return (a* temp + b * x2) / (temp + x2);
		case INTERPOLATION_METHOD_POWER3:
			return (a*pow(1 - x, 3) + b*pow(x, 3)) / (pow(1 - x, 3) + pow(x, 3));
		case INTERPOLATION_METHOD_JELAWAT:
			return a*(1 - 3 * pow(x, 2) + 2 * pow(x, 3)) + b*(3 * pow(x, 2) - 2 * pow(x, 3));
		default:
			return 0;
		}
	}

	double InterpolatedNoise(double x, double y){
		int int_X, int_Y;
		double fractional_X, fractional_Y, v1, v2, v3, v4, i1, i2;
		int_X = GoInt(x);
		fractional_X = x - int_X;
		int_Y = GoInt(y);
		fractional_Y = y - int_Y;
		if (NoiseSmoothed == 1){
			v1 = SmoothedNoise((double)int_X, (double)int_Y);
			v2 = SmoothedNoise((double)int_X + 1, (double)int_Y);
			v3 = SmoothedNoise((double)int_X, (double)int_Y + 1);
			v4 = SmoothedNoise((double)int_X + 1, (double)int_Y + 1);
		}
		else{
			v1 = Noise(int_X, int_Y);
			v2 = Noise(int_X + 1, int_Y);
			v3 = Noise(int_X, int_Y + 1);
			v4 = Noise(int_X + 1, int_Y + 1);
		}
		i1 = Interpolate(v1, v2, fractional_X);
		i2 = Interpolate(v3, v4, fractional_X);
		return Interpolate(i1, i2, fractional_Y);
	}

	double PerlinNoise2D(double x, double y){
		double total, frequency, amplitude, a;
		int i, n;
		total = 0;
		n = 8;
		for (i = 0; i <= n; i++){
			frequency = (double)pow(2, i);
			amplitude = (double)pow(0.5, i);
			a = InterpolatedNoise(x*frequency, y*frequency)*amplitude;
			total += a;
		}
		return total;
	}

	int getHeight(int x, int y){
		double NoiseHeight = PerlinNoise2D(x / NoiseScaleX, y / NoiseScaleZ) / 4.0f;
		return int(NoiseHeight);
	}

	int getSandHeight(int x, int y){
		double NoiseHeight = PerlinNoise2D(x / NoiseScaleX * 3 + 1222, y / NoiseScaleZ * 3 + 1222) / 4.0f - 2;
		return int(NoiseHeight);
	}
}