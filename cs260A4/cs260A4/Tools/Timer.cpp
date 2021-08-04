#include "Timer.h"

void Timer::Start()
{
	_time = std::chrono::steady_clock::now();
}

void Timer::Reset()
{
	_time = std::chrono::steady_clock::now();
}

float Timer::GetDuration()
{
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::duration<float> runtime = end - _time;
	return runtime.count();
}
