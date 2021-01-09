#include "Time.h"
#include <GLFW/glfw3.h>

double lastFrameTime = 0;
float deltaTime = 0.0f;

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
