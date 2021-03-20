#include "Time.h"
#include <GLFW/glfw3.h>

float Time::lastFrameTime = 0;
float Time::deltaTime = 0.0f;
uint64_t Time::frameCount = 0;

float Time::fixedUpdateDeltaTime = 0.02f;
float Time::fixedUpdateTimer = 0.0f;

float Time::TimeScale = 1.0f;

float Time::DeltaTime()
{
    return deltaTime * TimeScale;
}

uint64_t Time::FrameCount()
{
    return frameCount;
}

float Time::FixedDeltaTime()
{
    return fixedUpdateDeltaTime;
}

void Time::Update()
{
    auto currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    frameCount++;
}

bool Time::FixedUpdate()
{
    fixedUpdateTimer += deltaTime;

    if (fixedUpdateTimer >= fixedUpdateDeltaTime)
    {
        fixedUpdateTimer = 0.0f;
        return true;
    }

    return false;
}
