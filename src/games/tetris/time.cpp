#include "time.h"

Time::Time() :
    currTime{ ClockType::now() }
{
}

void Time::Start()
{
    currTime = ClockType::now();
}

float Time::Lap()
{
    const auto now = ClockType::now();
    const std::chrono::duration<float> dt = now - currTime;
    currTime = now;
    return dt.count();
}
