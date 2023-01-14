#pragma once

#include "Application.h"
#include "TimeProvider.h"

class Time
{
public:
    static void Init();
    static void Terminate();

    static void Update();
    static bool TryFixedUpdate();
    static void ResetFixedDeltaTime();

    static float DeltaTime();
    static float UnscaledDeltaTime();
    static float TimeSinceStartup();
    static uint64_t FrameCount();
    static float GetFixedDeltaTime();

    // TODO: apply to sound
    static float TimeScale;
    static float FixedDeltaTime;

private:
    static TimeProvider* CreateTimeProvider();

    static TimeProvider* timeProvider;
    static float lastFrameTime;
    static float lastFixedFrameTime;
    static float deltaTime;
    static uint64_t frameCount;

};