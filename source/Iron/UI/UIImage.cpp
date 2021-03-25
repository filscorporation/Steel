#include "UIImage.h"
#include "../Scene/SceneHelper.h"

void UIImage::SetImage(Sprite *image)
{
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

    if (!HasComponentS<RectTransformation>(Owner))
    {
        Log::LogError("UI entity has no RectTransformation attached");
        return;
    }

    if (_image == nullptr)
        GetComponentS<RectTransformation>(Owner).SetSize(glm::vec2(0.0f, 0.0f));
    else
        GetComponentS<RectTransformation>(Owner).SetSize(glm::vec2(_image->Width, _image->Height));
}

Sprite* UIImage::GetImage()
{
    return _image;
}
