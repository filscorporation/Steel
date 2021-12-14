#include "Renderer.h"
#include "Core/RenderContext.h"
#include "Core/RenderList.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "Steel/Rendering/Core/DrawCallsSource.h"

void Renderer::Init()
{
    Log::LogDebug("Begin renderer init");

    if (!OpenGLAPI::Init())
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

    renderTask.Source->CollectDrawCalls(&renderContext);
    renderContext.List.SortDrawCalls(&renderContext);
    renderContext.List.ExecuteDrawCalls(&renderContext);

    // Clear
    renderTask.TargetFramebuffer->Unbind();
}
