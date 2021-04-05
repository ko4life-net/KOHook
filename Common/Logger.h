#pragma once

#if defined(_DEBUG)
#    define ENABLE_LOGGING
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <memory>

namespace KOHook
{
    class Logger
    {
    public:
        static bool Init(const char * title);

        Logger(const Logger &) = delete;
        Logger & operator=(const Logger &) = delete;

        static std::shared_ptr<class spdlog::logger> & GetLogger();

    private:
        Logger() = default;

        static std::shared_ptr<class spdlog::logger> s_Instance;
    };
} // namespace KOHook

#if defined(ENABLE_LOGGING)
#    define LOG_TRACE(...)     SPDLOG_LOGGER_TRACE(KOHook::Logger::GetLogger().get(), __VA_ARGS__)
#    define LOG_INFO(...)      SPDLOG_LOGGER_INFO(KOHook::Logger::GetLogger().get(), __VA_ARGS__)
#    define LOG_WARN(...)      SPDLOG_LOGGER_WARN(KOHook::Logger::GetLogger().get(), __VA_ARGS__)
#    define LOG_ERROR(...)     SPDLOG_LOGGER_ERROR(KOHook::Logger::GetLogger().get(), __VA_ARGS__)
#    define LOG_CRITICAL(...)  SPDLOG_LOGGER_CRITICAL(KOHook::Logger::GetLogger().get(), __VA_ARGS__)
#else
#    define LOG_TRACE(...)
#    define LOG_INFO(...)
#    define LOG_WARN(...)
#    define LOG_ERROR(...)
#    define LOG_CRITICAL(...)
#endif