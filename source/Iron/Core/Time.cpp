#include "Time.h"
#include <GLFW/glfw3.h>

double Time::lastFrameTime = 0;
float Time::deltaTime = 0.0f;

float Time::DeltaTime()
{
    return deltaTime;
}

void Time::Update()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}
