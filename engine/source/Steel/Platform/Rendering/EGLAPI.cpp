#include "EGLAPI.h"
#include "Steel/Core/Log.h"
#include "Steel/Platform/Application/AndroidApplication.h"

EGLDisplay EGLAPI::_display;
EGLSurface EGLAPI::_surface;
int EGLAPI::_width;
int EGLAPI::_height;

int EGLAPI::Init()
{
    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (_display == EGL_NO_DISPLAY)
    {
        Log::LogError("EGL: No _display found");
        return -1;
    }

    EGLint versionMajor, versionMinor;
    if (!eglInitialize(_display, &versionMajor, &versionMinor))
    {
        Log::LogError("EGL: eglInitialise failed");
        return -1;
    }

    Log::LogDebug("EGL: initialized with version: {0}.{1}", versionMajor, versionMinor);

    const EGLint configAttributes[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_DEPTH_SIZE, 8,
        EGL_NONE
    };
    EGLint num_config;
    EGLConfig config;
    eglChooseConfig(_display, configAttributes, &config, 1, &num_config);

    const EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    EGLContext context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttributes);
    if (context == EGL_NO_CONTEXT)
    {
        Log::LogError("EGL: eglCreateContext failed: {0}", eglGetError());
        return -1;
    }

    auto nativeWindow = AndroidApplication::GetInstance()->GetNativeApplication()->window;
    _surface = eglCreateWindowSurface(_display, config, nativeWindow, nullptr);
    if (_surface == EGL_NO_SURFACE)
    {
        Log::LogError("EGL: eglCreateWindowSurface failed: {0}", eglGetError());
        return -1;
    }

    if (!eglQuerySurface(_display, _surface, EGL_WIDTH, &_width) ||
        !eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height))
    {
        Log::LogError("EGL: eglQuerySurface failed: {0}", eglGetError());
        return -1;
    }

    if (!eglMakeCurrent(_display, _surface, _surface, context))
    {
        Log::LogError("EGL: eglMakeCurrent() failed: {0}", eglGetError());
        return -1;
    }

    return 1;
}

void EGLAPI::SwapBuffers()
{
    eglSwapBuffers(_display, _surface);
}

glm::vec2 EGLAPI::GetDisplaySize()
{
    return { _width, _height };
}
