#include "UIImage.h"
#include "../UIQuadRenderer.h"
#include "../../Scene/SceneHelper.h"

void UIImage::UpdateRenderer(RectTransformation& transformation, bool transformationDirty)
{
    if (_image == nullptr)
        return;

    if (!transformationDirty)
        return;

    auto& qr = AddComponentS<UIQuadRenderer>(Owner);

    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    for (int i = 0; i < 4; ++i)
        qr.Vertices[i] = matrix * qr.DefaultVertices[i];
    qr.SortingOrder = transformation.GetGlobalSortingOrderCached();
}

void UIImage::SetImage(Sprite* image)
{
    bool wasNull = _image == nullptr;
    _image = image;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image == nullptr)
    {
        registry->RemoveComponent<UIQuadRenderer>(Owner);
    }
    else
    {
        auto& qr = registry->AddComponent<UIQuadRenderer>(Owner);
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
        qr.CustomOwner = Owner;
    }

    if (_image == nullptr)
        registry->GetComponent<RectTransformation>(Owner).SetSize(glm::vec2(0.0f, 0.0f));
    else if (wasNull)
    {
        glm::vec2 size;
        size.x = _image->IsSpriteSheet ? _image->TileWidth : _image->Width;
        size.y = _image->IsSpriteSheet ? _image->TileHeight : _image->Height;
        registry->GetComponent<RectTransformation>(Owner).SetSize(size);
    }
}

Sprite* UIImage::GetImage()
{
    return _image;
}
