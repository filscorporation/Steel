#pragma once

#include "Application.h"

class Time
{
public:
    static float DeltaTime();
private:
    static void Update();

    static double lastFrameTime;
    static float deltaTime;

    friend Application;
};