#include "Time.h"
#include <GLFW/glfw3.h>

float Time::lastFrameTime = 0;
float Time::lastFixedFrameTime = 0;
float Time::deltaTime = 0.0f;
uint64_t Time::frameCount = 0;

float Time::FixedDeltaTime = 0.02f;
float Time::TimeScale = 1.0f;

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
    return (float)glfwGetTime();
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
    auto currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    frameCount++;
}

bool Time::TryFixedUpdate()
{
    auto currentTime = (float)glfwGetTime();
    if (currentTime - lastFixedFrameTime >= FixedDeltaTime)
    {
        lastFixedFrameTime += FixedDeltaTime;
        return true;
    }

    return false;
}
