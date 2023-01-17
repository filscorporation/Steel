#pragma once

#include <spdlog/spdlog.h>

#ifdef PLATFORM_ANDROID
#include <spdlog/sinks/android_sink.h>
#endif

class Log
{
public:
    static void Init();

    template<typename FormatString, typename... Args>
    inline static void LogDebug(const FormatString& fmt, Args&&... args)
    {
#ifdef PLATFORM_ANDROID
        _logger->debug(fmt, std::forward<Args>(args)...);
#else
        spdlog::debug(fmt, std::forward<Args>(args)...);
#endif
    }

    template<typename FormatString, typename... Args>
    inline static void LogInfo(const FormatString& fmt, Args&&... args)
    {
#ifdef PLATFORM_ANDROID
        _logger->info(fmt, std::forward<Args>(args)...);
#else
        spdlog::info(fmt, std::forward<Args>(args)...);
#endif
    }

    template<typename FormatString, typename... Args>
    inline static void LogWarning(const FormatString& fmt, Args&&... args)
    {
#ifdef PLATFORM_ANDROID
        _logger->warn(fmt, std::forward<Args>(args)...);
#else
        spdlog::warn(fmt, std::forward<Args>(args)...);
#endif
    }

    template<typename FormatString, typename... Args>
    inline static void LogError(const FormatString& fmt, Args&&... args)
    {
#ifdef PLATFORM_ANDROID
        _logger->error(fmt, std::forward<Args>(args)...);
#else
        spdlog::error(fmt, std::forward<Args>(args)...);
#endif
    }

private:
#ifdef PLATFORM_ANDROID
    static std::shared_ptr<spdlog::logger> _logger;
#endif
};
