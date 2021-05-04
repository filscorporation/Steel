#include "UIImage.h"
#include "../UIQuadRenderer.h"
#include "../../Scene/SceneHelper.h"

void UIImage::UpdateRenderer(RectTransformation& transformation)
{
    if (_image == nullptr)
        return;

    auto& qr = GetComponentS<UIQuadRenderer>(Owner);

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
    auto rt = registry->GetComponent<RectTransformation>(Owner);
    if (_image == nullptr)
    {
        if (!wasNull)
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
        qr.SetDefaultQuad();
        qr.Color = _color;
        qr.TextureID = _image->TextureID;
        qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
        qr.CustomOwner = Owner;
    }

    if (_image == nullptr)
        rt.SetSize(glm::vec2(0.0f, 0.0f));
    else if (wasNull)
    {
        glm::vec2 size;
        size.x = _image->IsSpriteSheet ? _image->TileWidth : _image->Width;
        size.y = _image->IsSpriteSheet ? _image->TileHeight : _image->Height;
        rt.SetSize(size);
    }
}

Sprite* UIImage::GetImage()
{
    return _image;
}
