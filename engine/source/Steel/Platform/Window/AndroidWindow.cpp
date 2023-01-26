#include "AndroidWindow.h"
#include "Steel/Core/Application.h"

int _width, _height;
bool screenSizeDirty;

void AndroidWindow::Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync)
{
    _width = width;
    _height = height;
}

void AndroidWindow::InitInputHandler()
{

}

void AndroidWindow::Apply()
{
    _width = Application::Context()->ScreenParams.Width;
    _height = Application::Context()->ScreenParams.Height;
    Application::Context()->ScreenParams.ResolutionX = _width;
    Application::Context()->ScreenParams.ResolutionY = _height;
}

int AndroidWindow::GetWidth()
{
    return _width;
}

int AndroidWindow::GetHeight()
{
    return _height;
}

bool AndroidWindow::GetIsMinimized()
{
    return false;
}

bool AndroidWindow::IsScreenSizeDirty()
{
    return screenSizeDirty;
}

void AndroidWindow::SetScreenSizeDirty()
{
    screenSizeDirty = true;
}

void AndroidWindow::UpdateSize()
{

}

void AndroidWindow::SwapBuffers()
{

}

void AndroidWindow::ClearDirty()
{
    screenSizeDirty = false;
}

bool AndroidWindow::WindowShouldClose()
{
    return false;
}

void AndroidWindow::Terminate()
{

}

float AndroidWindow::InvertY(float y)
{
    return (float)_height - y;
}
