#include "AndroidTimeProvider.h"

#include <time.h>

double GetSystemTime()
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return (double)res.tv_sec + (double)res.tv_nsec / 1e9;
}

void AndroidTimeProvider::Init()
{
    startTime = GetSystemTime();
}

float AndroidTimeProvider::GetTime()
{
    return (float)(GetSystemTime() - startTime);
}
