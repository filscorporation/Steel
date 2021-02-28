#include "SpriteRenderer.h"
#include "../Core/Application.h"
#include "Renderer.h"

void SpriteRenderer::OnRender()
{
    if (ParentEntity->IsDestroyed())
        return;

    if (_image == nullptr)
        return;

    if (_image->IsSpriteSheet)
    {
        Renderer::DrawQuad(
                ParentEntity->Transform->GetTransformationMatrix(),
                _image->TextureID,
                _image->GetTexCoord(CurrentImageTileIndex));
    }
    else
    {
        Renderer::DrawQuad(ParentEntity->Transform->GetTransformationMatrix(), _image->TextureID);
    }
}

void SpriteRenderer::SetImage(Sprite *image)
{
    _image = image;
}

Sprite *SpriteRenderer::GetImage()
{
    return _image;
}

glm::vec2 SpriteRenderer::GetWorldSize()
{
    if (_image == nullptr)
        return glm::vec2(0, 0);

    // TODO: use sprite size
    return ParentEntity->Transform->GetScale();
}