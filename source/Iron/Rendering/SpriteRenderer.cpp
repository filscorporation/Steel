#include "SpriteRenderer.h"
#include "Renderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"

void SpriteRenderer::OnRender()
{
    if (_image == nullptr)
        return;

    auto& transform = GetComponentS<Transformation>(Owner);
    if (_image->IsSpriteSheet)
    {
        glm::vec2 texCoords[4];
        _image->GetTexCoord(CurrentImageTileIndex, texCoords);
        Renderer::DrawQuad(
                transform.GetTransformationMatrix(),
                _image->TextureID,
                texCoords);
    }
    else
    {
        Renderer::DrawQuad(transform.GetTransformationMatrix(), _image->TextureID);
    }
    // TODO: maybe not the best place to do that..
    transform.SetTransformationChanged(false);
}

void SpriteRenderer::SetImage(Sprite* image)
{
    _image = image;

    _isTransparent = _image != nullptr && _image->IsTransparent;
}

Sprite* SpriteRenderer::GetImage()
{
    return _image;
}

glm::vec2 SpriteRenderer::GetWorldSize()
{
    if (_image == nullptr)
        return glm::vec2(0, 0);

    // TODO: use sprite size
    return GetComponentS<Transformation>(Owner).GetScale();
}

bool SpriteRenderer::IsTransparent() const
{
    return _isTransparent;
}
