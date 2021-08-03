#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Color.h"
#include "Transform.h"
#include <math.h>
#include  <cmath>

namespace Math
{
	const float pi = 3.14159274f;
	const float epsilon = 0.001f;

	int Clamp(int value, int min, int max);
	float Clamp(float value, float min, float max);
	float Clamp01(float value);
	float MoveTowards(float a, float b, float t);
	float Lerp(float a, float b, float t);
	int RoundDown(float value);
	int RoundDownToNearest(float value, int multiple);
	float RoundDownf(float value);
	float RoundDownToNearestf(float value, int multiple);
	int Abs(int value);
	float Abs(float value);
}
