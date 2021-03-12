#pragma once

#include "Application.h"

class Time
{
public:
    static float DeltaTime();
    static float FixedDeltaTime();
    // TODO: apply to sound
    static float TimeScale;
private:
    static void Update();
    static bool FixedUpdate();

    static double lastFrameTime;
    static float deltaTime;

    static float fixedUpdateDeltaTime;
    static float fixedUpdateTimer;

    friend Application;
};