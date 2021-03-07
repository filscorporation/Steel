#include "Screen.h"
#include "../Core/Input.h"
#include "../Core/Application.h"
#include "../Rendering/Camera.h"
#include "../Core/Log.h"
#include <GLFW/glfw3.h>

#define UI_MIN_DISTANCE -0.01f
#define UI_MAX_DISTANCE 100.0f

GLFWwindow* _window;
bool needToUpdateViewport = false;
bool screenSizeDirty = false;

int Screen::_width, Screen::_height;
int Screen::_xPosition, Screen::_yPosition;
glm::vec3 Screen::_color;
glm::mat4 Screen::_viewProjection = glm::mat4(1.0f);
bool Screen::_fullscreen;
bool Screen::isInResizeCallback = false;

// TODO: currently window will not update if you held it still
// (possible solution is to separate poll_events and rendering in different threads)
void ResizeCallback(GLFWwindow* window, int cx, int cy);
void PositionCallback(GLFWwindow* window, int cx, int cy);

void Screen::Apply()
{
    if (_fullscreen)
    {
        glfwGetWindowPos(_window, &_xPosition, &_yPosition);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        glfwSetWindowMonitor(_window, nullptr, _xPosition, _yPosition, _width, _height, 0);
    }

    needToUpdateViewport = true;
}

int Screen::GetWidth()
{
    if (_fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempWidth;
    }

    return _width;
}

void Screen::SetWidth(int width)
{
    if (_width == width)
        return;

    _width = width;
    EnterCallback();
    Apply();
    ExitCallback();
}

int Screen::GetHeight()
{
    if (_fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempHeight;
    }

    return _height;
}

void Screen::SetHeight(int height)
{
    if (_height == height)
        return;

    _height = height;
    EnterCallback();
    Apply();
    ExitCallback();
}

bool Screen::GetFullscreen()
{
    return _fullscreen;
}

void Screen::SetFullscreen(bool fullscreen)
{
    if (_fullscreen == fullscreen)
        return;

    _fullscreen = fullscreen;
    EnterCallback();
    Apply();
    ExitCallback();
}

glm::vec3 Screen::GetColor()
{
    return _color;
}

void Screen::SetColor(glm::vec3 color)
{
    if (_color == color)
        return;

    _color = color;
}

glm::mat4 Screen::GetUIViewProjection()
{
    // TODO: maybe it should be in some UICamera component

    return _viewProjection;
}

bool Screen::IsScreenSizeDirty()
{
    return screenSizeDirty;
}

void Screen::UpdateUIViewProjection()
{
    glm::mat4 projection = glm::ortho(
            0.0f, (float)_width,
            0.0f, (float)_height,
            UI_MIN_DISTANCE, UI_MAX_DISTANCE
    );

    _viewProjection = projection;
}

void Screen::Init(int width, int height, glm::vec3 color, bool fullscreen)
{
    _fullscreen = fullscreen;
    _color = color;

    if (!glfwInit())
    {
        Log::LogError("Error initializing GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    if (_fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        _window = glfwCreateWindow(mode->width, mode->height, "Application", monitor, nullptr);
    }
    else
    {
        _window = glfwCreateWindow(width, height, "Application", nullptr, nullptr);
    }
    glfwMakeContextCurrent(_window);

    glfwGetWindowPos(_window, &_xPosition, &_yPosition);
    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, _width, _height);

    glfwSetWindowSizeCallback(_window, ResizeCallback);
    glfwSetWindowPosCallback(_window, PositionCallback);

    UpdateUIViewProjection();

    Log::LogInfo("Initialized screen with size " + std::to_string(_width) + ":" + std::to_string(_height));

    Input::Init(_window);
}

void ResizeCallback(GLFWwindow* window, int cx, int cy)
{
    if (!Screen::IsInCallback())
    {
        needToUpdateViewport = true;
        screenSizeDirty = true;
        Screen::EnterCallback();
        Application::Instance->RunUpdate();
        Screen::ExitCallback();
    }
}

void PositionCallback(GLFWwindow* window, int cx, int cy)
{
    if (!Screen::IsInCallback())
    {
        needToUpdateViewport = true;
        Screen::EnterCallback();
        Application::Instance->RunUpdate();
        Screen::ExitCallback();
    }
}

void Screen::UpdateSize()
{
    if (needToUpdateViewport)
    {
        needToUpdateViewport = false;
        int tempWidth, tempHeight;
        int tempX, tempY;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);
        glfwGetWindowPos(_window, &tempX, &tempY);
        if (!_fullscreen)
        {
            _width = tempWidth;
            _height = tempHeight;
            _xPosition = tempX;
            _yPosition = tempY;
        }
        glViewport(0, 0, tempWidth, tempHeight);

        UpdateUIViewProjection();
        Application::Instance->GetCurrentScene()->GetMainCamera()->UpdateSize();
    }
}

void Screen::SwapBuffers()
{
    glfwSwapBuffers(_window);

    screenSizeDirty = false;
}

bool Screen::WindowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Screen::Terminate()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

bool Screen::IsInCallback()
{
    return isInResizeCallback;
}

void Screen::EnterCallback()
{
    isInResizeCallback = true;
}

void Screen::ExitCallback()
{
    isInResizeCallback = false;
}
