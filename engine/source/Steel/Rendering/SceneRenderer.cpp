#include "SceneRenderer.h"
#include "Camera.h"
#include "Renderer.h"
#include "SceneDrawCallsSource.h"
#include "Screen.h"
#include "UIDrawCallsSource.h"
#include "Core/RenderTask.h"
#include "Steel/Core/Application.h"
#include "Steel/Scene/Scene.h"

void SceneRenderer::Draw(Scene* scene, Framebuffer* framebuffer)
{
    // TODO: rework stats collecting
    Application::Context()->Stats.DrawCalls = 0;
    Application::Context()->Stats.VerticesCount = 0;

    auto cameras = scene->GetEntitiesRegistry()->GetComponentIterator<Camera>();
    // TODO: sort cameras by draw order
    // Render scene cameras
    for (int i = 0; i < cameras.Size(); ++i)
    {
        if (!cameras[i].IsAlive())
            continue;

        RenderTask renderTask;
        SceneDrawCallsSource drawCallsSource(scene, cameras[i].GetRenderMask());
        renderTask.Source = &drawCallsSource;
        renderTask.ViewProjection = cameras[i].GetViewProjection();
        renderTask.TargetFramebuffer = cameras[i].GetCustomTargetFramebuffer() != nullptr
                ? cameras[i].GetCustomTargetFramebuffer()
                : framebuffer;
        renderTask.ClearFlagsValue = cameras[i].GetClearFlag();
        Renderer::Draw(renderTask);
    }

    // Render UI
    {
        RenderTask renderTask;
        UIDrawCallsSource drawCallsSource(scene);
        renderTask.Source = &drawCallsSource;
        renderTask.ViewProjection = Screen::GetUIViewProjection();
        renderTask.TargetFramebuffer = framebuffer;
        renderTask.ClearFlagsValue = ClearFlags::Depth | ClearFlags::Stencil;
        Renderer::Draw(renderTask);
    }
}
