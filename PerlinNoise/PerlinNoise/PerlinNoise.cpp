#include "PerlinNoise.h"

namespace Effects
{
	Effects::PerlinNoise::PerlinNoise()
	{
		size = 1024;
		permutationTable = new char[size];
		for (size_t i = 0; i < size; i++)
			permutationTable[i] = std::rand();
	}

	float Effects::PerlinNoise::GetNoise(float x, float y) const
	{
		//индекс ближайшей левой €чейки сетки
		int x0 = static_cast<int>(floorf(x));
		//индекс ближайшей верхней €чейки сетки
		int y0 = static_cast<int>(floorf(y));

		//индекс правой €чейки сетки
		int x1 = x0 + 1;
		//индекс нижней €чейки
		int y1 = y0 + 1;

		//дельта [0, 1] переход от одной €чейки к другой
		float deltaX = x - static_cast<float>(x0);
		float deltaY = y - static_cast<float>(y0);

		float n0 = DotGridGradient(x0, y0, deltaX, deltaY);
		float n1 = DotGridGradient(x1, y0, deltaX - 1, deltaY);

		float n2 = DotGridGradient(x0, y1, deltaX, deltaY - 1);
		float n3 = DotGridGradient(x1, y1, deltaX - 1, deltaY - 1);

		float deltaX2 = QuinticLerp(deltaX);

		//линейна€ интерпол€ци€
		float ix0 = Lerp(n0, n1, deltaX2);
		float ix1 = Lerp(n2, n3, deltaX2);

		float deltaY2 = QuinticLerp(deltaY);

		float value = Lerp(ix0, ix1, deltaY2);

		return value;
	}

	float PerlinNoise::GetNoise(float x, float y, int octaves, float persistence) const
	{
		float amplitude = 1;
		float max = 0;
		float result = 0;

		while (octaves > 0)
		{
			octaves--;

			max += amplitude;
			result += GetNoise(x, y) * amplitude;
			amplitude *= persistence;

			x *= 2;
			y *= 2;
		}

		result /= max;

		return result;
	}

	void Effects::PerlinNoise::GetPseudoRandomVector(int x, int y, float& gx, float& gy) const
	{
		int v = static_cast<int>(((x * 1836311903) ^ (y * 2971215073) + 4807526976) & 1023);
		v = permutationTable[v] & 3;

		switch (v)
		{
		case 0:
			gx = 1;
			gy = 0;
			return;
		case 1:
			gx = -1;
			gy = 0;
			return;
		case 2:
			gx = 0;
			gy = 1;
			return;
		default:
			gx = 0;
			gy = -1;
			return;
		}

		/*gx = sin(x);
		gy = cos(y);*/
	}

	float PerlinNoise::DotGridGradient(int gridX, int gridY, float toCenterX, float toCenterY) const
	{
		float gradX, gradY;
		GetPseudoRandomVector(gridX, gridY, gradX, gradY);
		float dotProduct = Dot(gradX, gradY, toCenterX, toCenterY);
		return dotProduct;
	}

	float PerlinNoise::Dot(float x0, float x1, float y0, float y1)
	{
		return x0 * y0 + x1 * y1;
	}

	float PerlinNoise::Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	float PerlinNoise::QuinticLerp(float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}
}