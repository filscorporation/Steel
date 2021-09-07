#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Screen.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Log.h"

GLFWwindow* _window;
bool needToUpdateViewport = false;
bool screenSizeDirty = false;

Framebuffer* Screen::_framebuffer = nullptr;
int Screen::_xPosition, Screen::_yPosition;
glm::mat4 Screen::_viewProjection = glm::mat4(1.0f);
bool Screen::_fullscreen;
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
    if (_fullscreen)
    {
        glfwGetWindowPos(_window, &_xPosition, &_yPosition);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        glfwSetWindowMonitor(_window, nullptr, _xPosition, _yPosition,
                             Application::Context()->ScreenParameters.Width,
                             Application::Context()->ScreenParameters.Height, 0);
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

    return Application::Context()->ScreenParameters.ResolutionX;
}

void Screen::SetWidth(int width)
{
    if (!Application::Context()->ScreenParameters.CanResize || Application::Context()->ScreenParameters.Width == width)
        return;

    Application::Instance->ScreenParametersForUpdate().Width = width;
    Application::Instance->ScreenParametersForUpdate().ResolutionX = width;
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

    return Application::Context()->ScreenParameters.ResolutionY;
}

void Screen::SetHeight(int height)
{
    if (!Application::Context()->ScreenParameters.CanResize || Application::Context()->ScreenParameters.Height == height)
        return;

    Application::Instance->ScreenParametersForUpdate().Height = height;
    Application::Instance->ScreenParametersForUpdate().ResolutionY = height;
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
    if (!Application::Context()->ScreenParameters.CanResize || _fullscreen == fullscreen)
        return;

    _fullscreen = fullscreen;
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
    return Application::Context()->ScreenParameters.Color;
}

void Screen::SetColor(glm::vec3 color)
{
    if (Application::Context()->ScreenParameters.Color == color)
        return;

    Application::Instance->ScreenParametersForUpdate().Color = color;
}

glm::mat4 Screen::GetUIViewProjection()
{
    return _viewProjection;
}

bool Screen::IsScreenSizeDirty()
{
    return screenSizeDirty;
}

void Screen::UpdateUIViewProjection(int width, int height)
{
    glm::mat4 projection = glm::ortho(
            0.0f, (float)width,
            0.0f, (float)height,
            -1.0f, UI_MAX_DISTANCE
    );

    _viewProjection = projection;
}

void Screen::Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer)
{
    _fullscreen = fullscreen;
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
    glfwGetFramebufferSize(_window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetWindowSizeCallback(_window, ResizeCallback);
    glfwSetWindowPosCallback(_window, PositionCallback);

    UpdateUIViewProjection(width, height);

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
    if (!Application::Context()->ScreenParameters.CanResize)
        return;

    if (needToUpdateViewport)
    {
        needToUpdateViewport = false;
        int tempWidth, tempHeight;
        int tempX, tempY;
        glfwGetFramebufferSize(_window, &tempWidth, &tempHeight);
        glfwGetWindowPos(_window, &tempX, &tempY);
        if (!_fullscreen)
        {
            Application::Instance->ScreenParametersForUpdate().Width = tempWidth;
            Application::Instance->ScreenParametersForUpdate().Height = tempHeight;
            _xPosition = tempX;
            _yPosition = tempY;

            _isMinimized = Application::Context()->ScreenParameters.Width == 0 && Application::Context()->ScreenParameters.Height == 0;
        }
        Application::Instance->ScreenParametersForUpdate().ResolutionX = Application::Context()->ScreenParameters.Width;
        Application::Instance->ScreenParametersForUpdate().ResolutionY = Application::Context()->ScreenParameters.Height;
        glViewport(0, 0, tempWidth, tempHeight);

        Input::ReleaseAllEvents();
        UpdateUIViewProjection(Application::Context()->ScreenParameters.Width, Application::Context()->ScreenParameters.Height);
        Application::Instance->GetCurrentScene()->GetMainCamera().UpdateSize();
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
    return (position - glm::vec2(Application::Context()->ScreenParameters.OffsetX, Application::Context()->ScreenParameters.OffsetY))
        / glm::vec2(Application::Context()->ScreenParameters.Width, Application::Context()->ScreenParameters.Height)
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
