#include "UILetterRenderer.h"
#include "../Rendering/Renderer.h"

void UILetterRenderer::Render()
{
    if (!IsRendered)
        return;

    Renderer::DrawQuadCached(Cache, Color, TextureID, TextureCoords);
}
