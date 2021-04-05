#pragma once

#include <timeapi.h>

class Timer
{
public:
    enum class Command
    {
        Reset,           // reset the timer
        Start,           // start the timer
        Stop,            // stop (or pause) the timer
        Advance,         // advance the timer by 0.1 seconds
        GetAbsoluteTime, // get the absolute system time
        GetAppTime,      // get the current time
        GetElapsedTime   // get the time that elapsed between
    };

    static float Process(Command command);

private:
    static BOOL     s_bTimerInitialized;
    static BOOL     s_bUsingQPF;
    static LONGLONG s_llQPFTicksPerSec;
};
