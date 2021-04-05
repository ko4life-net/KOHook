#pragma once

#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace KOHook
{

    void ExitProgram(char exitCode);

    int GetProcessPID(const std::wstring & processName);

    bool LoggerInit(const char * title);

    template <typename... Args>
    void PrintLog(const char * fmt, Args &&... args)
    {
#ifdef _DEBUG
        std::stringstream ss;
        tm                time;
        auto timet = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        localtime_s(&time, &timet);
        ss << "[" << std::put_time(&time, "%Y-%m-%d %X") << "]";
        ss << " [DEBUG]  ";
        ss << fmt;
        ss << "\n";
        std::string fmted = ss.str();
        printf(fmted.c_str(), std::forward<Args>(args)...);
#endif
    }

} // namespace KOHook