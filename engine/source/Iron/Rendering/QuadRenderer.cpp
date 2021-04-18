#include "QuadRenderer.h"
#include "Renderer.h"

void QuadRenderer::Render()
{
    Renderer::DrawQuad(Vertices, TextureCoords, Color, TextureID);
}
