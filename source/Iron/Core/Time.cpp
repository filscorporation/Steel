#include "Time.h"
#include <GLFW/glfw3.h>

float Time::lastFrameTime = 0;
float Time::deltaTime = 0.0f;

float Time::fixedUpdateDeltaTime = 0.02f;
float Time::fixedUpdateTimer = 0.0f;

float Time::TimeScale = 1.0f;

float Time::DeltaTime()
{
    return deltaTime * TimeScale;
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
