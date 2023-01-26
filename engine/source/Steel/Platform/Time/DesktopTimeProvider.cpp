#include "DesktopTimeProvider.h"

#include <GLFW/glfw3.h>

void DesktopTimeProvider::Init()
{

}

float DesktopTimeProvider::GetTime()
{
    return (float)glfwGetTime();
}
