#include "timer.h"

#include <ctime>


uint64_t Timer::unixTime()
{
    return static_cast<uint64_t>(
        std::time(nullptr)
    );
}



int Timer::secondsRemaining(uint64_t period)
{
    uint64_t current =
        unixTime();


    uint64_t elapsed =
        current % period;


    return static_cast<int>(
        period - elapsed
    );
}