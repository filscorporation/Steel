#include "DesktopWindow.h"
#include "Steel/Core/Log.h"
#include "Steel/Input/Input.h"
#include "Steel/Core/Application.h"
#include "Steel/Platform/Input/DesktopInputHandler.h"

#include <GLFW/glfw3.h>

GLFWwindow* _window;
bool needToUpdateViewport = false;
bool screenSizeDirty = true;

// TODO: currently window will not update if you held it still
// (possible solution is to separate poll_events and rendering in different threads)
void ResizeCallback(GLFWwindow* window, int cx, int cy);
void PositionCallback(GLFWwindow* window, int cx, int cy);

void DesktopWindow::Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync)
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

    if (!vSync) glfwSwapInterval(0);

    glfwGetWindowPos(_window, &_xPosition, &_yPosition);
    glfwGetFramebufferSize(_window, &width, &height);

    glfwSetWindowSizeCallback(_window, ResizeCallback);
    glfwSetWindowPosCallback(_window, PositionCallback);
}

void DesktopWindow::InitInputHandler()
{
    DesktopInputHandler::Init(_window);
}

void DesktopWindow::Apply()
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

int DesktopWindow::GetWidth()
{
    if (Application::Context()->ScreenParams.Fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempWidth;
    }

    return Application::Context()->ScreenParams.ResolutionX;
}

void DesktopWindow::SetWidth(int width)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Width == width)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Width = width;
    Application::Context()->ScreenParams.WidthBackup = width;
    Application::Context()->ScreenParams.ResolutionX = width;
    Screen::EnterCallback();
    Apply();
    Screen::ExitCallback();
}

int DesktopWindow::GetHeight()
{
    if (Application::Context()->ScreenParams.Fullscreen)
    {
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);

        return tempHeight;
    }

    return Application::Context()->ScreenParams.ResolutionY;
}

void DesktopWindow::SetHeight(int height)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Height == height)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Height = height;
    Application::Context()->ScreenParams.HeightBackup = height;
    Application::Context()->ScreenParams.ResolutionY = height;
    Screen::EnterCallback();
    Apply();
    Screen::ExitCallback();
}

bool DesktopWindow::GetFullscreen()
{
    return Application::Context()->ScreenParams.Fullscreen;
}

void DesktopWindow::SetFullscreen(bool fullscreen)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Fullscreen == fullscreen)
        return;

    screenSizeDirty = true;
    Application::Context()->ScreenParams.Fullscreen = fullscreen;
    Screen::EnterCallback();
    Apply();
    Screen::ExitCallback();
}

bool DesktopWindow::GetIsMinimized()
{
    return _isMinimized;
}

bool DesktopWindow::IsScreenSizeDirty()
{
    return screenSizeDirty;
}

void DesktopWindow::SetScreenSizeDirty()
{
    screenSizeDirty = true;
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

void DesktopWindow::UpdateSize()
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

void DesktopWindow::SwapBuffers()
{
    if (_doubleBuffer)
        glfwSwapBuffers(_window);
}

void DesktopWindow::ClearDirty()
{
    screenSizeDirty = false;
}

bool DesktopWindow::WindowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void DesktopWindow::Terminate()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

float DesktopWindow::InvertY(float y)
{
    int tempWidth, tempHeight;
    glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);
    return (float)tempHeight - y;
}
