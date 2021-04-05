#include "pch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace KOHook;

std::shared_ptr<spdlog::logger> Logger::s_Instance;

bool Logger::Init(const char * title)
{
#if defined(ENABLE_LOGGING)
    static bool bConsoleInit = false;

    if (bConsoleInit) return true;

    bConsoleInit = AllocConsole();
    SetConsoleTitleA(title);
    FILE * p;
    freopen_s(&p, "CONIN$", "r", stdin);
    freopen_s(&p, "CONOUT$", "w", stdout);
    freopen_s(&p, "CONOUT$", "w", stderr);

    // spdlog
    const char * pattern = "%^[%D %H:%M:%S %e] [%l] [%!::%#]:%$ %v";
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("KOHook.log", false));
    for (auto && sink : logSinks)
        sink->set_pattern(pattern);
    spdlog::set_pattern(pattern);
    spdlog::set_level(spdlog::level::trace);

    // Instance logger
    s_Instance = std::make_shared<spdlog::logger>(title, logSinks.begin(), logSinks.end());
    s_Instance->set_level(spdlog::level::trace);
    s_Instance->flush_on(spdlog::level::trace);
    spdlog::register_logger(s_Instance);

    return bConsoleInit;
#else
    return true;
#endif // ENABLE_LOGGING
}

std::shared_ptr<spdlog::logger> & Logger::GetLogger()
{
    return s_Instance;
}
