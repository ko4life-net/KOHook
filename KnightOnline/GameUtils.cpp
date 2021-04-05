#include "pch.h"
#include "GameUtils.h"

BOOL     Timer::s_bTimerInitialized = FALSE;
BOOL     Timer::s_bUsingQPF         = FALSE;
LONGLONG Timer::s_llQPFTicksPerSec  = 0;

// Based on DXUtil_Timer()
float Timer::Process(Timer::Command command)
{
    // Initialize the timer
    if (s_bTimerInitialized == FALSE)
    {
        s_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        s_bUsingQPF = QueryPerformanceFrequency(&qwTicksPerSec);
        if (s_bUsingQPF) s_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if (s_bUsingQPF)
    {
        static LONGLONG s_llStopTime        = 0;
        static LONGLONG s_llLastElapsedTime = 0;
        static LONGLONG s_llBaseTime        = 0;
        double          fTime;
        double          fElapsedTime;
        LARGE_INTEGER   qwTime;

        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if (s_llStopTime != 0 && command != Command::Start &&
            command != Command::GetAbsoluteTime)
            qwTime.QuadPart = s_llStopTime;
        else
            QueryPerformanceCounter(&qwTime);

        // Return the elapsed time
        if (command == Command::GetElapsedTime)
        {
            fElapsedTime =
                (double)(qwTime.QuadPart - s_llLastElapsedTime) / (double)s_llQPFTicksPerSec;
            s_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT)fElapsedTime;
        }

        // Return the current time
        if (command == Command::GetAppTime)
        {
            double fAppTime =
                (double)(qwTime.QuadPart - s_llBaseTime) / (double)s_llQPFTicksPerSec;
            return (FLOAT)fAppTime;
        }

        // Reset the timer
        if (command == Command::Reset)
        {
            s_llBaseTime        = qwTime.QuadPart;
            s_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }

        // Start the timer
        if (command == Command::Start)
        {
            s_llBaseTime += qwTime.QuadPart - s_llStopTime;
            s_llStopTime        = 0;
            s_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }

        // Stop the timer
        if (command == Command::Stop)
        {
            s_llStopTime        = qwTime.QuadPart;
            s_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }

        // Advance the timer by 1/10th second
        if (command == Command::Advance)
        {
            s_llStopTime += s_llQPFTicksPerSec / 10;
            return 0.0f;
        }

        if (command == Command::GetAbsoluteTime)
        {
            fTime = qwTime.QuadPart / (double)s_llQPFTicksPerSec;
            return (FLOAT)fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double s_fLastElapsedTime = 0.0;
        static double s_fBaseTime        = 0.0;
        static double s_fStopTime        = 0.0;
        double        fTime;
        double        fElapsedTime;

        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if (s_fStopTime != 0.0 && command != Command::Start &&
            command != Command::GetAbsoluteTime)
            fTime = s_fStopTime;
        else
            fTime = timeGetTime() * 0.001;

        // Return the elapsed time
        if (command == Command::GetElapsedTime)
        {
            fElapsedTime       = (double)(fTime - s_fLastElapsedTime);
            s_fLastElapsedTime = fTime;
            return (FLOAT)fElapsedTime;
        }

        // Return the current time
        if (command == Command::GetAppTime) { return (FLOAT)(fTime - s_fBaseTime); }

        // Reset the timer
        if (command == Command::Reset)
        {
            s_fBaseTime        = fTime;
            s_fLastElapsedTime = fTime;
            return 0.0f;
        }

        // Start the timer
        if (command == Command::Start)
        {
            s_fBaseTime += fTime - s_fStopTime;
            s_fStopTime        = 0.0f;
            s_fLastElapsedTime = fTime;
            return 0.0f;
        }

        // Stop the timer
        if (command == Command::Stop)
        {
            s_fStopTime = fTime;
            return 0.0f;
        }

        // Advance the timer by 1/10th second
        if (command == Command::Advance)
        {
            s_fStopTime += 0.1f;
            return 0.0f;
        }

        if (command == Command::GetAbsoluteTime) { return (FLOAT)fTime; }

        return -1.0f; // Invalid command specified
    }
}
