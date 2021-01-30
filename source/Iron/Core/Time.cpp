#include "Time.h"
#include <GLFW/glfw3.h>

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
