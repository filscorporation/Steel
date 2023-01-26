#include "Time.h"
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    #include "Steel/Platform/Time/DesktopTimeProvider.h"
#endif
#if defined PLATFORM_ANDROID
    #include "Steel/Platform/Time/AndroidTimeProvider.h"
#endif

TimeProvider* Time::timeProvider = nullptr;
float Time::lastFrameTime = 0;
float Time::lastFixedFrameTime = 0;
float Time::deltaTime = 0.0f;
uint64_t Time::frameCount = 0;

float Time::FixedDeltaTime = 0.02f;
float Time::TimeScale = 1.0f;

void Time::Init()
{
    timeProvider = CreateTimeProvider();
    timeProvider->Init();
}

void Time::Terminate()
{
    delete timeProvider;
}

float Time::DeltaTime()
{
    return deltaTime * TimeScale;
}

float Time::UnscaledDeltaTime()
{
    return deltaTime;
}

float Time::TimeSinceStartup()
{
    return timeProvider->GetTime();
}

uint64_t Time::FrameCount()
{
    return frameCount;
}

float Time::GetFixedDeltaTime()
{
    return FixedDeltaTime * TimeScale;
}

void Time::Update()
{
    auto currentTime = timeProvider->GetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    frameCount++;
}

bool Time::TryFixedUpdate()
{
    auto currentTime = timeProvider->GetTime();
    if (currentTime - lastFixedFrameTime >= FixedDeltaTime)
    {
        lastFixedFrameTime += FixedDeltaTime;
        return true;
    }

    return false;
}

void Time::ResetFixedDeltaTime()
{
    lastFixedFrameTime = timeProvider->GetTime() - FixedDeltaTime;
}

TimeProvider* Time::CreateTimeProvider()
{
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    return new DesktopTimeProvider();
#endif
#if defined PLATFORM_ANDROID
    return new AndroidTimeProvider();
#endif
}
