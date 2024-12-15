#pragma once

#include <chrono>

class Time
{
public:
    using ClockType = std::chrono::steady_clock;

    Time();

    void Start();
    float Lap();

private:
    std::chrono::time_point<ClockType> currTime;
};
