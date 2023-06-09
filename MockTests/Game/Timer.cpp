#include "Timer.h"

Timer::Timer()
{
    time = std::chrono::high_resolution_clock::now();
}

float Timer::Tick()
{
    auto curTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> elapsedTime = curTime - time;

    time = curTime;

    return elapsedTime.count();
}

void Timer::Reset()
{
    time = std::chrono::high_resolution_clock::now();
}
