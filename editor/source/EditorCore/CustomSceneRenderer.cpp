#include "CustomSceneRenderer.h"
#include "EditorBuilder.h"
#include "EditorApplication.h"

#include <Steel/Rendering/Renderer.h>

void CustomSceneRenderer::DrawScene()
{
    _framebuffer->Bind();
    Renderer::Clear(STYLE_SCENE_VIEW_CLEAR_COLOR);

    if (_cameraEntity == NULL_ENTITY)
        return;

    BeforeDraw();

    // Camera from editor scene (not from rendered scene)
    auto editor = (EditorApplication*)Application::Instance;
    auto& camera = editor->EditorContext->Scenes->GetActiveScene()->GetEntitiesRegistry()->GetComponent<Camera>(_cameraEntity);

    // Normal render pass
    RenderPass(camera);

    // Wireframe render pass (objects polygon lines for debugging)
    Renderer::BeginWireframeMode();
    RenderPass(camera);
    Renderer::EndWireframeMode();

    AfterDraw();

    _framebuffer->Unbind();
}

void CustomSceneRenderer::RenderPass(Camera& camera)
{
    Renderer::OnBeforeRender(camera);

    auto quadRenderers = _scene->GetEntitiesRegistry()->GetComponentIterator<QuadRenderer>();

    // Opaque pass
    for (int i = 0; i < quadRenderers.Size(); ++i)
        if (quadRenderers[i].Queue == RenderingQueue::Opaque)
            Renderer::Draw(quadRenderers[i]);

    // Transparent pass
    for (int i = quadRenderers.Size() - 1; i >= 0; --i)
        if (quadRenderers[i].Queue == RenderingQueue::Transparent)
            Renderer::Draw(quadRenderers[i]);

    Renderer::OnAfterRender();
}
