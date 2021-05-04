#include "QuadRenderer.h"
#include "Renderer.h"

void QuadRenderer::Render()
{
    Renderer::DrawQuad(Vertices, TextureCoords, Color, TextureID);
}

void QuadRenderer::SetDefaultQuad()
{
    DefaultVertices[0] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    DefaultVertices[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
    DefaultVertices[2] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
    DefaultVertices[3] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
}

void QuadRenderer::SetDefaultQuad(glm::vec2 pivot)
{
    DefaultVertices[0] = glm::vec4(1.0f - pivot.x, 1.0f - pivot.y, 0.0f, 1.0f);
    DefaultVertices[1] = glm::vec4(1.0f - pivot.x, 0.0f - pivot.y, 0.0f, 1.0f);
    DefaultVertices[2] = glm::vec4(0.0f - pivot.x, 1.0f - pivot.y, 0.0f, 1.0f);
    DefaultVertices[3] = glm::vec4(0.0f - pivot.x, 0.0f - pivot.y, 0.0f, 1.0f);
}
