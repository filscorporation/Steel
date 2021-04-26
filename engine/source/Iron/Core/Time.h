#pragma once

#include "Application.h"

class Time
{
public:
    static float DeltaTime();
    static float UnscaledDeltaTime();
    static uint64_t FrameCount();
    static float FixedDeltaTime();
    // TODO: apply to sound
    static float TimeScale;
private:
    static void Update();
    static bool FixedUpdate();

    static float lastFrameTime;
    static float deltaTime;
    static uint64_t frameCount;

    static float fixedUpdateDeltaTime;
    static float fixedUpdateTimer;

    friend Application;
};