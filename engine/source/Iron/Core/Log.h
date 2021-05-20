#pragma once

#include <spdlog/spdlog.h>

class Log
{
public:
    static void Init();

    template<typename FormatString, typename... Args>
    inline static void LogDebug(const FormatString& fmt, Args&&... args)
    {
        spdlog::debug(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    inline static void LogInfo(const FormatString& fmt, Args&&... args)
    {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    inline static void LogWarning(const FormatString& fmt, Args&&... args)
    {
        spdlog::warn(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    inline static void LogError(const FormatString& fmt, Args&&... args)
    {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }
};