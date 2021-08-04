#pragma once

#include <chrono>

class Timer
{
public:
	void Start();
	void Reset();
	float GetDuration();
private:
	std::chrono::steady_clock::time_point _time;
};