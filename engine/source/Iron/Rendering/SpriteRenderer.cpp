#include "SpriteRenderer.h"
#include "Renderer.h"
#include "../Scene/SceneHelper.h"

void SpriteRenderer::OnRender(Transformation& transformation)
{
    if (_image == nullptr)
        return;

    if (_image->IsSpriteSheet)
    {
        glm::vec2 texCoords[4];
        _image->GetTexCoord(CurrentImageTileIndex, texCoords);
        if (transformation.DidTransformationChange())
        {
            glm::mat4 matrix = transformation.GetTransformationMatrixCached()
                * glm::scale(glm::mat4(1.0f), _image->GetRealWorldSize());
            Renderer::DrawQuad(
                    quadCache,
                    matrix,
                    _color,
                    _image->TextureID,
                    texCoords);
        }
        else
            Renderer::DrawQuadCached(
                    quadCache,
                    _color,
                    _image->TextureID,
                    texCoords);
    }
    else
    {
        if (transformation.DidTransformationChange())
        {
            glm::mat4 matrix = transformation.GetTransformationMatrixCached()
                               * glm::scale(glm::mat4(1.0f), _image->GetRealWorldSize());
            Renderer::DrawQuad(quadCache, matrix, _color, _image->TextureID);
        }
        else
            Renderer::DrawQuadCached(quadCache, _color, _image->TextureID);
    }

    transformation.SetTransformationChanged(false);
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

    return GetComponentS<Transformation>(Owner).GetScale() * _image->GetRealWorldSize();
}

bool SpriteRenderer::IsTransparent() const
{
    return _isTransparent;
}
