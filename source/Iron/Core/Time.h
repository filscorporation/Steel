#pragma once

#include "Application.h"

class Time
{
public:
    static float DeltaTime();
    // TODO: apply to sound
    static float TimeScale;
private:
    static void Update();

    static double lastFrameTime;
    static float deltaTime;

    friend Application;
};