#pragma once

struct DRData
{
	DRData() = default;
	DRData(float accel_x, float accel_y, float gametime_, int player_index)
		:accelx{accel_x}, accely{accel_y}, gametime{gametime_}, playerindex{player_index}
	{

	}
	float accelx;
	float accely;
	float gametime;
	int playerindex;
};
