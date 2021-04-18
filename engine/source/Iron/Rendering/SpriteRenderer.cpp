#include "SpriteRenderer.h"
#include "Renderer.h"
#include "QuadRenderer.h"
#include "../Scene/SceneHelper.h"

void SpriteRenderer::UpdateRenderer(Transformation& transformation)
{
    if (_image == nullptr)
        return;

    auto& qr = GetComponentS<QuadRenderer>(Owner);

    glm::mat4 matrix = transformation.GetTransformationMatrixCached()
                       * glm::scale(glm::mat4(1.0f), _image->GetRealWorldSize());
    for (int i = 0; i < 4; ++i)
        qr.Vertices[i] = matrix * qr.DefaultVertices[i];
    qr.SortingOrder = transformation.GetGlobalSortingOrderCached();

    imageDirty = false;
}

void SpriteRenderer::SetImage(Sprite* image)
{
    _image = image;
    _isTransparent = _image != nullptr && _image->IsTransparent;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image == nullptr)
    {
        registry->RemoveComponent<QuadRenderer>(Owner);
    }
    else
    {
        auto& qr = registry->AddComponent<QuadRenderer>(Owner);
        if (_image->IsSpriteSheet)
        {
            _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
        }
        else
        {
            qr.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
            qr.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
            qr.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
            qr.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
        }
        qr.DefaultVertices[0] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        qr.DefaultVertices[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        qr.DefaultVertices[2] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
        qr.DefaultVertices[3] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
        qr.Color = _color;
        qr.TextureID = _image->TextureID;
        qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;

        imageDirty = true;
    }
}

Sprite* SpriteRenderer::GetImage()
{
    return _image;
}

void SpriteRenderer::SetTileIndex(uint32_t tileIndex)
{
    if (tileIndex == currentImageTileIndex)
        return;

    if (_image == nullptr || !_image->IsSpriteSheet)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& qr = registry->AddComponent<QuadRenderer>(Owner);

    currentImageTileIndex = tileIndex;
    _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
}

uint32_t SpriteRenderer::GetTileIndex() const
{
    return currentImageTileIndex;
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
