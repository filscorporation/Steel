#include "SpriteRenderer.h"
#include "../Core/Application.h"
#include "Renderer.h"

void SpriteRenderer::OnRender()
{
    if (_image == nullptr)
        return;

    Renderer::BindTexture(_image->TextureID);
    Renderer::DrawQuad(ParentObject->Transform->GetTransformationMatrix());
}

void SpriteRenderer::SetImage(Image *image)
{
    _image = image;
}
