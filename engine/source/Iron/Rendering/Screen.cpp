#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Screen.h"
#include "OpenGLAPI.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Log.h"

GLFWwindow* _window;
bool needToUpdateViewport = false;
bool screenSizeDirty = true;

Framebuffer* Screen::_framebuffer = nullptr;
int Screen::_xPosition, Screen::_yPosition;
bool Screen::_isMinimized = false;
bool Screen::_doubleBuffer = true;
bool Screen::isInResizeCallback = false;

// TODO: currently window will not update if you held it still
// (possible solution is to separate poll_events and rendering in different threads)
void ResizeCallback(GLFWwindow* window, int cx, int cy);
void PositionCallback(GLFWwindow* window, int cx, int cy);

Framebuffer* Screen::ScreenFramebuffer()
{
    return _framebuffer;
}

void Screen::Apply()
{
    if (Application::Context()->ScreenParams.Fullscreen)
    {
        glfwGetWindowPos(_window, &_xPosition, &_yPosition);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, 0);
        Application::Context()->ScreenParams.WidthBackup = Application::Context()->ScreenParams.Width;
        Application::Context()->ScreenParams.HeightBackup = Application::Context()->ScreenParams.Height;
        Application::Context()->ScreenParams.Width = mode->width;
        Application::Context()->ScreenParams.Width = mode->width;
        Application::Context()->ScreenParams.Height = mode->height;
        if (Application::Context()->ScreenParams.AutoResolution)
        {
            Application::Context()->ScreenParams.ResolutionX = mode->width;
            Application::Context()->ScreenParams.ResolutionY = mode->height;
        }
    }
    else
    {
        Application::Context()->ScreenParams.Width = Application::Context()->ScreenParams.WidthBackup;
        Application::Context()->ScreenParams.Height = Application::Context()->ScreenParams.HeightBackup;
        if (Application::Context()->ScreenParams.AutoResolution)
        {
            Application::Context()->ScreenParams.ResolutionX = Application::Context()->ScreenParams.WidthBackup;
            Application::Context()->ScreenParams.ResolutionY = Application::Context()->ScreenParams.HeightBackup;
        }
        if (Application::Context()->ScreenParams.CanResize)
        {
            glfwSetWindowMonitor(_window, nullptr, _xPosition, _yPosition,
                                 Application::Context()->ScreenParams.Width,
                                 Application::Context()->ScreenParams.Height, 0);
        }
    }

    needToUpdateViewport = true;
}

int Screen::GetWidth()
{
    if (Application::Context()->ScreenParams.Fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempWidth;
    }

    return Application::Context()->ScreenParams.ResolutionX;
}

void Screen::SetWidth(int width)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Width == width)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Width = width;
    Application::Context()->ScreenParams.WidthBackup = width;
    Application::Context()->ScreenParams.ResolutionX = width;
    EnterCallback();
    Apply();
    ExitCallback();
}

int Screen::GetHeight()
{
    if (Application::Context()->ScreenParams.Fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempHeight;
    }

    return Application::Context()->ScreenParams.ResolutionY;
}

void Screen::SetHeight(int height)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Height == height)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Height = height;
    Application::Context()->ScreenParams.HeightBackup = height;
    Application::Context()->ScreenParams.ResolutionY = height;
    EnterCallback();
    Apply();
    ExitCallback();
}

bool Screen::GetFullscreen()
{
    return Application::Context()->ScreenParams.Fullscreen;
}

void Screen::SetFullscreen(bool fullscreen)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Fullscreen == fullscreen)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Fullscreen = fullscreen;
    EnterCallback();
    Apply();
    ExitCallback();
}

bool Screen::GetIsMinimized()
{
    return _isMinimized;
}

glm::vec3 Screen::GetColor()
{
    return Application::Context()->ScreenParams.Color;
}

void Screen::SetColor(glm::vec3 color)
{
    if (Application::Context()->ScreenParams.Color == color)
        return;

    Application::Context()->ScreenParams.Color = color;
}

glm::mat4 Screen::GetUIViewProjection()
{
    return glm::ortho(
            0.0f, (float)Application::Context()->ScreenParams.ResolutionX,
            0.0f, (float)Application::Context()->ScreenParams.ResolutionY,
            -1.0f, UI_MAX_DISTANCE
    );
}

bool Screen::IsScreenSizeDirty()
{
    return screenSizeDirty;
}

void Screen::SetScreenSizeDirty()
{
    screenSizeDirty = true;
}

void Screen::Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer)
{
    _doubleBuffer = doubleBuffer;

    if (!glfwInit())
    {
        Log::LogError("Error initializing GLFW");
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, _doubleBuffer);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    if (fullscreen)
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
    glfwGetFramebufferSize(_window, &width, &height);

    glfwSetWindowSizeCallback(_window, ResizeCallback);
    glfwSetWindowPosCallback(_window, PositionCallback);

    _framebuffer = new Framebuffer();

    Log::LogDebug("Initialized screen with size {0}:{1}", width, height);

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
        Input::ReleaseAllEvents();
        Application::Instance->RunUpdate();
        Screen::ExitCallback();
    }
}

void Screen::UpdateSize()
{
    if (!Application::Context()->ScreenParams.CanResize)
        return;

    if (needToUpdateViewport)
    {
        needToUpdateViewport = false;
        int tempWidth, tempHeight;
        int tempX, tempY;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);
        glfwGetWindowPos(_window, &tempX, &tempY);
        if (!Application::Context()->ScreenParams.Fullscreen)
        {
            Application::Context()->ScreenParams.Width = tempWidth;
            Application::Context()->ScreenParams.Height = tempHeight;
            _xPosition = tempX;
            _yPosition = tempY;

            _isMinimized = Application::Context()->ScreenParams.Width == 0 && Application::Context()->ScreenParams.Height == 0;
        }
        Application::Context()->ScreenParams.ResolutionX = Application::Context()->ScreenParams.Width;
        Application::Context()->ScreenParams.ResolutionY = Application::Context()->ScreenParams.Height;

        Input::ReleaseAllEvents();
    }
}

void Screen::SwapBuffers()
{
    if (_doubleBuffer)
        glfwSwapBuffers(_window);
    else
        glFlush();

    screenSizeDirty = false;
}

bool Screen::WindowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Screen::Terminate()
{
    delete _framebuffer;

    glfwDestroyWindow(_window);
    glfwTerminate();
}

glm::vec2 Screen::Transform(const glm::vec2& position)
{
    return (position - glm::vec2(Application::Context()->ScreenParams.OffsetX, Application::Context()->ScreenParams.OffsetY))
        / glm::vec2(Application::Context()->ScreenParams.Width, Application::Context()->ScreenParams.Height)
        * glm::vec2(Screen::GetWidth(), Screen::GetHeight());
}

float Screen::InvertY(float y)
{
    int tempWidth, tempHeight;
    glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);
    return (float)tempHeight - y;
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
