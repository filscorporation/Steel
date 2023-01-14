#include "DesktopTimeProvider.h"

#include <GLFW/glfw3.h>

float DesktopTimeProvider::GetTime()
{
    return (float)glfwGetTime();
}
