#include "WorldGen.h"
#include "Def.h"
//Perlin Noise 2D;
namespace WorldGen{
	float perm[256];
	int   seed;
	int   NoiseSmoothed = 1;
	int   InterpolationMethod = INTERPOLATION_METHOD_POWER2;
	float NoiseScaleX = 32;
	float NoiseScaleY = (float)1 / 4;
	float NoiseScaleZ = 32;
	int   MountainLevel = 36;
	float MountainScale = 1;
	int   WaterLevel = 30;
	int   SandLevel = 32;

	void perlinNoiseInit(int mapseed){
		srand(mapseed);
		int i;
		for (i = 0; i != 256; i++){
			perm[i] = (float)(rnd() * 256);
		}
		seed = mapseed;
	}

	float Noise(int x, int y){
		return perm[abs(x * 437 + y) % 256];
	}

	float SmoothedNoise(float x, float y){
		float corners, sides, center;
		int intx = static_cast<int>(x);
		int inty = static_cast<int>(y);
		corners = (Noise(intx - 1, inty - 1) + Noise(intx + 1, inty - 1) + Noise(intx - 1, inty + 1) + Noise(intx + 1, inty + 1)) / 32;
		sides = (Noise(intx - 1, inty) + Noise(intx + 1, inty) + Noise(intx, inty - 1) + Noise(intx, inty + 1)) / 16;
		center = Noise(intx, inty) / 8;
		return corners + sides + center;
	}

	float Interpolate(float a, float b, float x){
		switch (InterpolationMethod){
		case INTERPOLATION_METHOD_LINEAR:
			return a*(1 - x) + b*(x);
		case INTERPOLATION_METHOD_COSINE:
			return float(a*(1 - (1 - cos(x*M_PI))*0.5) + b*(1 - cos(x*M_PI))*0.5);
		case INTERPOLATION_METHOD_POWER2:
			return (a*pow(1 - x, 2) + b*pow(x, 2)) / (pow(1 - x, 2) + pow(x, 2));
		case INTERPOLATION_METHOD_POWER3:
			return (a*pow(1 - x, 3) + b*pow(x, 3)) / (pow(1 - x, 3) + pow(x, 3));
		case INTERPOLATION_METHOD_JELAWAT:
			return a*(1 - 3 * pow(x, 2) + 2 * pow(x, 3)) + b*(3 * pow(x, 2) - 2 * pow(x, 3));
		default:
			return 0;
		}
	}

	float InterpolatedNoise(float x, float y){
		int int_X, int_Y;
		float fractional_X, fractional_Y, v1, v2, v3, v4, i1, i2;
		int_X = int(x);
		fractional_X = x - int_X;
		int_Y = int(y);
		fractional_Y = y - int_Y;
		if (NoiseSmoothed == 1){
			v1 = SmoothedNoise((float)int_X, (float)int_Y);
			v2 = SmoothedNoise((float)int_X + 1, (float)int_Y);
			v3 = SmoothedNoise((float)int_X, (float)int_Y + 1);
			v4 = SmoothedNoise((float)int_X + 1, (float)int_Y + 1);
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

	float PerlinNoise2D(float x, float y){
		float total, frequency, amplitude, a;
		int i, n;
		total = 0;
		n = 8;
		for (i = 0; i <= n; i++){
			frequency = (float)pow(2, i);
			amplitude = (float)pow(0.5, i);
			a = InterpolatedNoise(x*frequency, y*frequency)*amplitude;
			total += a;
		}
		return total;
	}

	int getHeight(int x, int y){
		float NoiseHeight = PerlinNoise2D(x / NoiseScaleX, y / NoiseScaleZ)*NoiseScaleY;
		return int(NoiseHeight);
	}

	int getSandHeight(int x, int y){
		float NoiseHeight = PerlinNoise2D(x / NoiseScaleX * 3 + 1222, y / NoiseScaleZ * 3 + 1222) / 4.0f - 2;
		return int(NoiseHeight);
	}
}