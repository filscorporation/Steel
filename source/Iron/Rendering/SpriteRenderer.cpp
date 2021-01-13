#include "SpriteRenderer.h"
#include "../Core/Application.h"
#include "Renderer.h"

void SpriteRenderer::OnRender()
{
    if (_image == nullptr)
        return;

    if (_image->IsSpriteSheet)
    {
        Renderer::DrawQuad(
                ParentObject->Transform->GetTransformationMatrix(),
                _image->TextureID,
                _image->GetTexCoord(CurrentImageTileIndex));
    }
    else
    {
        Renderer::DrawQuad(ParentObject->Transform->GetTransformationMatrix(), _image->TextureID);
    }
}

void SpriteRenderer::SetImage(Sprite *image)
{
    _image = image;
}
