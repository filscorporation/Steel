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
}

void SpriteRenderer::SetMaterial(Material* material)
{
    _material = material;
    SetImage(_image);
}

Material* SpriteRenderer::GetMaterial()
{
    return _material;
}

void SpriteRenderer::SetCustomProperties(const MaterialPropertyBlock& properties)
{
    _customProperties = properties;
    _customProperties.UpdateHash();

    if (_image == nullptr || _material == nullptr)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& qr = registry->AddComponent<QuadRenderer>(Owner);
    qr.CustomProperties = _customProperties;
}

const MaterialPropertyBlock& SpriteRenderer::GetCustomProperties()
{
    return _customProperties;
}

void SpriteRenderer::SetImage(Sprite* image)
{
    _customProperties.UpdateHash();

    if (image != nullptr && _material == nullptr)
        _material = Application::Instance->GetResourcesManager()->DefaultSpriteMaterial();

    bool wasNull = _image == nullptr;
    _image = image;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image == nullptr)
    {
        if (!wasNull)
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
        qr.SetDefaultQuad(_image->Pivot);
        qr.Color = _color;
        qr.RenderMaterial = _material;
        if (_image->SpriteTexture != nullptr)
            _customProperties.SetTexture(MAIN_TEX, _image->SpriteTexture->GetTextureID());
        qr.CustomProperties = _customProperties;
        qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
    }
}

Sprite* SpriteRenderer::GetImage()
{
    return _image;
}

void SpriteRenderer::SetImageTileIndex(uint32_t index)
{
    if (index == currentImageTileIndex)
        return;

    if (_image == nullptr || !_image->IsSpriteSheet)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& qr = registry->AddComponent<QuadRenderer>(Owner);

    currentImageTileIndex = index;
    _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
}

uint32_t SpriteRenderer::GetImageTileIndex() const
{
    return currentImageTileIndex;
}

glm::vec2 SpriteRenderer::GetWorldSize()
{
    if (_image == nullptr)
        return glm::vec2(0, 0);

    return GetComponentS<Transformation>(Owner).GetScale() * _image->GetRealWorldSize();
}
