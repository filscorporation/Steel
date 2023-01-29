#include "Log.h"

#ifdef PLATFORM_ANDROID
#include <spdlog/sinks/android_sink.h>
#endif

#ifdef PLATFORM_ANDROID
std::shared_ptr<spdlog::logger> Log::_logger = nullptr;
#endif

void Log::Init()
{
#if defined DISTRIBUTE_BUILD && !defined PLATFORM_ANDROID
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif

#ifdef PLATFORM_ANDROID
    std::string tag = "AppSteel";
    _logger = spdlog::android_logger_mt("Steel", tag);
#endif
}
