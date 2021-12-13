#include "EditorSceneRenderer.h"
#include "EditorApplication.h"
#include "Steel/Core/Application.h"
#include "Steel/Scene/Scene.h"
#include "Steel/Rendering/Camera.h"
#include "Steel/Rendering/Renderer.h"
#include "Steel/Rendering/SceneDrawCallsSource.h"
#include "Steel/Rendering/Screen.h"
#include "Steel/Rendering/UIDrawCallsSource.h"
#include "Steel/Rendering/Core/RenderTask.h"

void EditorSceneRenderer::DrawToSceneView(Scene* scene, Framebuffer* framebuffer, EntityID cameraEntity)
{
    // Camera from editor scene (not from rendered scene)
    auto editor = (EditorApplication*)Application::Instance;
    auto& camera = editor->EditorContext->Scenes->GetActiveScene()->GetEntitiesRegistry()->GetComponent<Camera>(cameraEntity);

    {
        RenderTask renderTask;
        renderTask.Source = new SceneDrawCallsSource(scene, camera.GetRenderMask());
        renderTask.ViewProjection = camera.GetViewProjection();
        renderTask.TargetFramebuffer = framebuffer;
        renderTask.ClearFlagsValue = camera.GetClearFlag();
        Renderer::Draw(renderTask);
        delete renderTask.Source;
    }

    // Wireframe render pass (objects polygon lines for debugging)
    /*Renderer::BeginWireframeMode();
    RenderPass(camera);
    Renderer::EndWireframeMode();*/

    // Render UI
    /*
    {
        RenderTask renderTask;
        renderTask.Source = new UIDrawCallsSource(scene);
        renderTask.ViewProjection = Screen::GetUIViewProjection();
        renderTask.TargetFramebuffer = framebuffer;
        renderTask.ClearFlagsValue = ClearFlags::Depth | ClearFlags::Stencil;
        Renderer::Draw(renderTask);
        delete renderTask.Source;
    }*/
}
