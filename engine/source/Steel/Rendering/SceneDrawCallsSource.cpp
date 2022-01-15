#include "SceneDrawCallsSource.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

SceneDrawCallsSource::SceneDrawCallsSource(Scene* scene, RenderMask mask)
{
    _scene = scene;
    _mask = mask;
}

void SceneDrawCallsSource::CollectDrawCalls(RenderContext* renderContext)
{
    auto spriteRenderers = _scene->GetEntitiesRegistry()->GetComponentIterator<SpriteRenderer>();
    renderContext->List.Reserve(spriteRenderers.Size());
    for (int i = 0; i < spriteRenderers.Size(); ++i)
        spriteRenderers[i].Draw(renderContext);

    auto meshRenderers = _scene->GetEntitiesRegistry()->GetComponentIterator<MeshRenderer>();
    renderContext->List.Reserve(meshRenderers.Size());
    for (int i = 0; i < meshRenderers.Size(); ++i)
        meshRenderers[i].Draw(renderContext);
}
