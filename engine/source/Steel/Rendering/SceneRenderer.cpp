#include "SceneRenderer.h"
#include "Renderer.h"
#include "Screen.h"

SceneRenderer::SceneRenderer(Scene* scene, Framebuffer* framebuffer, EntityID cameraEntity)
{
    _scene = scene;
    _framebuffer = framebuffer;
    _cameraEntity = cameraEntity;
}

void SceneRenderer::DrawScene()
{
    _framebuffer->Bind();
    Renderer::Clear(Screen::GetColor());

    if (_cameraEntity == NULL_ENTITY)
        return;

    BeforeDraw();

    auto& camera = _scene->entitiesRegistry->GetComponent<Camera>(_cameraEntity);
    Renderer::OnBeforeRender(camera);

    auto quadRenderers = _scene->entitiesRegistry->GetComponentIterator<QuadRenderer>();

    // Opaque pass
    for (int i = 0; i < quadRenderers.Size(); ++i)
        if (quadRenderers[i].Queue == RenderingQueue::Opaque)
            Renderer::Draw(quadRenderers[i]);

    // Transparent pass
    for (int i = quadRenderers.Size() - 1; i >= 0; --i)
        if (quadRenderers[i].Queue == RenderingQueue::Transparent)
            Renderer::Draw(quadRenderers[i]);

    // Clear depth buffer before rendering UI
    Renderer::PrepareUIRender();
    // Draw UI on top
    // TODO: move all UILayer draw logic here
    _scene->uiLayer->Draw();

    AfterDraw();

    Renderer::OnAfterRender();
    _framebuffer->Unbind();
}

void SceneRenderer::BeforeDraw()
{
    _scene->BeforeDraw();
}

void SceneRenderer::AfterDraw()
{
    _scene->AfterDraw();
}
