#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <glm/vec2.hpp>

#undef EGLAPI

class EGLAPI
{
public:
    static int Init();
    static void SwapBuffers();
    static glm::vec2 GetDisplaySize();

private:
    static EGLDisplay _display;
    static EGLSurface _surface;

    static int _width;
    static int _height;
};
