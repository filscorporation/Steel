#include "Screen.h"
#include "../Core/Input.h"
#include "../Core/Application.h"
#include "../Rendering/Camera.h"
#include "../Core/Log.h"
#include <GLFW/glfw3.h>

GLFWwindow* window;
int _width, _height;
glm::vec3 _color;
bool _fullscreen;

int Screen::GetWidth()
{
    return _width;
}

int Screen::GetHeight()
{
    return _height;
}

glm::vec3 Screen::Color()
{
    return _color;
}

void Screen::Init(int width, int height, glm::vec3 color, bool fullscreen)
{
    _fullscreen = fullscreen;
    _color = color;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if (_fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window = glfwCreateWindow(mode->width, mode->height, "Application", monitor, nullptr);
    }
    else
    {
        window = glfwCreateWindow(width, height, "Application", nullptr, nullptr);
    }
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &_width, &_height);
    glViewport(0, 0, _width, _height);

    Log::LogInfo("Initialized screen with size " + std::to_string(_width) + ":" + std::to_string(_height));

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
    glfwDestroyWindow(window);
    glfwTerminate();
}

glm::vec2 Screen::ScreenToWorldPosition(glm::vec2 position)
{
    auto camera = Application::Instance->GetCurrentScene()->MainCamera;
    return glm::vec2(
            camera->GetWidth() * (position.x / float(_width) - 0.5)
            + camera->ParentEntity->Transform->GetPosition().x,
            camera->GetHeight() * ((float(_height) - position.y) / float(_height) - 0.5)
            + camera->ParentEntity->Transform->GetPosition().y);
}
