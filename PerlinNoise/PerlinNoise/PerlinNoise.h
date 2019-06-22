#pragma once
#include<cmath>

namespace  Effects
{
	class PerlinNoise
	{
	public:
		PerlinNoise();
		float GetNoise(float x, float y) const;
		float GetNoise(float x, float y, int octaves, float persistence = 0.5f) const;
	protected:
		virtual void GetPseudoRandomVector(int x, int y, float& gx, float& gy) const;
	private:
		char* permutationTable;
		int size;

		static float Dot(float x0, float x1, float y0, float y1);
		static float Lerp(float a, float b, float t);
		static float QuinticLerp(float t);
		float DotGridGradient(int gridX, int gridY, float toCenterX, float toCenterY) const;
	};
}