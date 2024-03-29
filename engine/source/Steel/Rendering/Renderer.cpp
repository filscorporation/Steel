#include "Renderer.h"
#include "Screen.h"
#include "Core/RenderContext.h"
#include "Core/RenderList.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "Steel/Rendering/Core/DrawCallsSource.h"

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
#include "Steel/Platform/Rendering/GLADAPI.h"
#endif
#if defined PLATFORM_ANDROID
#include "Steel/Platform/Rendering/EGLAPI.h"
#endif

void Renderer::Init()
{
    Log::LogDebug("Begin renderer init");

    int initAPI = -1;

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    initAPI = GLADAPI::Init();
#endif
#if defined PLATFORM_ANDROID
    initAPI = EGLAPI::Init();
#endif

    if (!initAPI)
    {
        Log::LogError("Error loading OpenGL");
        return;
    }

    OpenGLAPI::EnableDepthTest();
    OpenGLAPI::EnableStencilTest();
    OpenGLAPI::EnableBlend();

    Log::LogDebug("Renderer initialized");
}

void Renderer::Terminate()
{

}

void Renderer::Draw(RenderTask& renderTask)
{
    // Prepare
    renderTask.TargetFramebuffer->Bind();

    // Collect and execute draw calls
    RenderContext renderContext;
    renderContext.Task = &renderTask;
    renderContext.List.ClearColor = Screen::GetColor();

    renderTask.Source->CollectDrawCalls(&renderContext);
    renderContext.List.SortDrawCalls(&renderContext);
    renderContext.List.ExecuteDrawCalls(&renderContext);

    // Clear
    renderTask.TargetFramebuffer->Unbind();
}
