#include "Screen.h"
#include "../Core/Input.h"
#include <GLFW/glfw3.h>

GLFWwindow* window;
int _width, _height;
bool _fullscreen;

int Screen::Width()
{
    return _width;
}

int Screen::Height()
{
    return _height;
}

void Screen::Init(int width, int height, bool fullscreen)
{
    _fullscreen = fullscreen;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, "Application", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &_width, &_height);
    glViewport(0, 0, width, height);

    Input::Init(window);
}

void Screen::SwapBuffers()
{
    glfwSwapBuffers(window);
}

bool Screen::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Screen::Terminate()
{
    glfwTerminate();
}
