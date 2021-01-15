#include "Screen.h"
#include "../Core/Input.h"
#include "../Core/Application.h"
#include "../Rendering/Camera.h"
#include <GLFW/glfw3.h>

GLFWwindow* window;
int width, height;
glm::vec3 color;
bool fullscreen;

int Screen::Width()
{
    return width;
}

int Screen::Height()
{
    return height;
}

glm::vec3 Screen::Color()
{
    return color;
}

void Screen::Init(int widthP, int heightP, glm::vec3 colorP, bool fullscreenP)
{
    fullscreen = fullscreenP;
    color = colorP;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(widthP, heightP, "Application", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &width, &height);
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

glm::vec2 Screen::ScreenToWorldPosition(glm::vec2 position)
{
    auto camera = Application::Instance->GetCurrentScene()->MainCamera;
    return glm::vec2(camera->Width * (position.x / float(width) - 0.5), camera->Height * ((float(height) - position.y) / float(height) - 0.5));
}
