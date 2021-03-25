#include "UILetterRenderer.h"
#include "../Rendering/Renderer.h"

void UILetterRenderer::Render()
{
    if (!IsRendered)
        return;

    Renderer::DrawQuadCached(QuadCache, Color, TextureID, TextureCoords);
}
