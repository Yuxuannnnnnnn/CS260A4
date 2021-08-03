#include "Math.h"

namespace Math
{
	int Clamp(int value, int min, int max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	float Clamp01(float value)
	{
		if (value < 0)
			return 0;
		if (value > 1)
			return 1;
		return value;
	}

	float MoveTowards(float a, float b, float t)
	{
		return a < b ? a + t : a - t;
	}

	float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	int RoundDown(float value)
	{
		return static_cast<int>(value);
	}

	int RoundDownToNearest(float value, int multiple)
	{
		return static_cast<int>(value) - static_cast<int>(value) % multiple;
	}

	float RoundDownf(float value)
	{
		return static_cast<float>(static_cast<int>(value));
	}

	float RoundDownToNearestf(float value, int multiple)
	{
		return static_cast<float>(static_cast<int>(value) - static_cast<int>(value) % multiple);
	}

	int Abs(int value)
	{
		return value < 0 ? -value : value;
	}

	float Abs(float value)
	{
		return value < 0 ? -value : value;
	}
}