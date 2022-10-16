#pragma once

#include "Application.h"

class Time
{
public:
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

    static float lastFrameTime;
    static float lastFixedFrameTime;
    static float deltaTime;
    static uint64_t frameCount;

};