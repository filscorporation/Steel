#include "UIImage.h"
#include "../Scene/SceneHelper.h"

void UIImage::SetImage(Sprite *image)
{
    bool wasNull = _image == nullptr;
    _image = image;

    if (!HasComponentS<UIRenderer>(Owner))
    {
        Log::LogError("UI entity has no UI renderer attached");
        return;
    }
    auto& uii = GetComponentS<UIRenderer>(Owner);

    uii.IsRendered = _image != nullptr;
    uii.TextureID = _image == nullptr ? 0 : _image->TextureID;
    uii.IsTransparent = _image != nullptr && _image->IsTransparent;

    if (_image->IsSpriteSheet)
    {
        _image->GetTexCoord(0, uii.TextureCoords);
    }

    if (!HasComponentS<RectTransformation>(Owner))
    {
        Log::LogError("UI entity has no RectTransformation attached");
        return;
    }

    if (_image == nullptr)
        GetComponentS<RectTransformation>(Owner).SetSize(glm::vec2(0.0f, 0.0f));
    else if (wasNull)
    {
        glm::vec2 size;
        size.x = _image->IsSpriteSheet ? _image->TileWidth : _image->Width;
        size.y = _image->IsSpriteSheet ? _image->TileHeight : _image->Height;
        GetComponentS<RectTransformation>(Owner).SetSize(size);
    }
}

Sprite* UIImage::GetImage()
{
    return _image;
}
