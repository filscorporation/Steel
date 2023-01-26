#include "Screen.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    #include "Steel/Platform/Window/DesktopWindow.h"
#endif
#if defined PLATFORM_ANDROID
    #include "Steel/Platform/Window/AndroidWindow.h"
#endif

#include <glm/gtc/matrix_transform.hpp>

Window* Screen::_window = nullptr;

Framebuffer* Screen::_framebuffer = nullptr;
bool Screen::_doubleBuffer = false;
bool Screen::isInResizeCallback = false;

void Screen::Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync)
{
    _doubleBuffer = doubleBuffer;

    _window = CreateWindow();
    _window->Init(width, height, color, fullscreen, doubleBuffer, vSync);

    _framebuffer = new Framebuffer();

    Log::LogDebug("Initialized screen with size {0}:{1}", width, height);
}

void Screen::InitInputHandler()
{
    _window->InitInputHandler();
}

void Screen::Apply()
{
    _window->Apply();
}

Framebuffer* Screen::ScreenFramebuffer()
{
    return _framebuffer;
}

void Screen::Resize(int width, int height)
{
    if (!Application::Context()->ScreenParams.CanResize
        || Application::Context()->ScreenParams.Width == width && Application::Context()->ScreenParams.Height == height)
        return;

    _window->SetScreenSizeDirty();
    Application::Context()->ScreenParams.Width = width;
    Application::Context()->ScreenParams.Height = height;
    Application::Context()->ScreenParams.WidthBackup = width;
    Application::Context()->ScreenParams.HeightBackup = height;
    Application::Context()->ScreenParams.ResolutionX = width;
    Application::Context()->ScreenParams.ResolutionY = height;
    EnterCallback();
    Apply();
    ExitCallback();
}

int Screen::GetWidth()
{
    return _window->GetWidth();
}

void Screen::SetWidth(int width)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Width == width)
        return;

    _window->SetScreenSizeDirty();
    Application::Context()->ScreenParams.Width = width;
    Application::Context()->ScreenParams.WidthBackup = width;
    Application::Context()->ScreenParams.ResolutionX = width;
    EnterCallback();
    Apply();
    ExitCallback();
}

int Screen::GetHeight()
{
    return _window->GetHeight();
}

void Screen::SetHeight(int height)
{
    if (!Application::Context()->ScreenParams.CanResize || Application::Context()->ScreenParams.Height == height)
        return;

    _window->SetScreenSizeDirty();
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

    _window->SetScreenSizeDirty();
    Application::Context()->ScreenParams.Fullscreen = fullscreen;
    EnterCallback();
    Apply();
    ExitCallback();
}

bool Screen::GetIsMinimized()
{
    return _window->GetIsMinimized();
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
            -1.0f, 1.0f
    );
}

bool Screen::IsScreenSizeDirty()
{
    return _window->IsScreenSizeDirty();
}

void Screen::SetScreenSizeDirty()
{
    _window->SetScreenSizeDirty();
}

void Screen::UpdateSize()
{
    if (!Application::Context()->ScreenParams.CanResize)
        return;

    _window->UpdateSize();
}

void Screen::SwapBuffers()
{
    if (_doubleBuffer)
        _window->SwapBuffers();
    else
        OpenGLAPI::Flush();

    _window->ClearDirty();
}

bool Screen::WindowShouldClose()
{
    return _window->WindowShouldClose();
}

void Screen::Terminate()
{
    delete _framebuffer;

    _window->Terminate();
    delete _window;
}

glm::vec2 Screen::Transform(const glm::vec2& position)
{
    return (position - glm::vec2(Application::Context()->ScreenParams.OffsetX, Application::Context()->ScreenParams.OffsetY))
        / glm::vec2(Application::Context()->ScreenParams.Width, Application::Context()->ScreenParams.Height)
        * glm::vec2(Screen::GetWidth(), Screen::GetHeight());
}

float Screen::InvertY(float y)
{
    return _window->InvertY(y);
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

Window* Screen::CreateWindow()
{
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    return new DesktopWindow();
#endif
#if defined PLATFORM_ANDROID
    return new AndroidWindow();
#endif
}
