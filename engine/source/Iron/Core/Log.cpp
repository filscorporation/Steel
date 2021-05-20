#include "Log.h"

void Log::Init()
{
#ifdef DISTRIBUTE_BUILD
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif
}
