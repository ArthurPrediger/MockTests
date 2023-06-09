#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	float Tick();
	void Reset();
private:
	std::chrono::steady_clock::time_point time;
};