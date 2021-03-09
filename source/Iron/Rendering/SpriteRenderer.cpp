#include "SpriteRenderer.h"
#include "Renderer.h"
#include "../Core/Application.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"

void SpriteRenderer::OnRender()
{
    if (Application::Instance->GetCurrentScene()->IsEntityDestroyed(Owner))
        return;

    if (_image == nullptr)
        return;

    if (_image->IsSpriteSheet)
    {
        glm::vec2 texCoords[4];
        _image->GetTexCoord(CurrentImageTileIndex, texCoords);
        Renderer::DrawQuad(
                GetComponentS<Transformation>(Owner).GetTransformationMatrix(),
                _image->TextureID,
                texCoords);
    }
    else
    {
        Renderer::DrawQuad(GetComponentS<Transformation>(Owner).GetTransformationMatrix(), _image->TextureID);
    }
}

void SpriteRenderer::SetImage(Sprite* image)
{
    _image = image;
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
