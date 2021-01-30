#pragma once

#include "Application.h"

class Time
{
public:
    static float DeltaTime();
private:
    static void Update();

    static double lastFrameTime = 0;
    static float deltaTime = 0.0f;

    friend Application;
};