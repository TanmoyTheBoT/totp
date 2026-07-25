#ifndef TIMER_H
#define TIMER_H

#include <cstdint>


class Timer
{
public:

    // Return seconds remaining in current TOTP period
    static int secondsRemaining(
        uint64_t period = 30
    );


    // Return current Unix timestamp
    static uint64_t unixTime();

};

#endif