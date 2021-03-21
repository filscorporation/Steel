#include "UIImage.h"
#include "../Scene/SceneHelper.h"
#include "UIEventHandler.h"

UIImage::UIImage(EntityID ownerEntityID) : UIComponent(ownerEntityID)
{
    if (HasComponentS<UIRenderer>(ownerEntityID))
    {
        Log::LogError("Entity already has UIRenderer component attached, this may lead to conflicts. Keep one UIComponent per object");
    }
    else
    {
        AddComponentS<UIRenderer>(ownerEntityID);
    }

    if (HasComponentS<UIEventHandler>(ownerEntityID))
    {
        Log::LogError("Entity already has UIEventHandler component attached, this may lead to conflicts. Keep one UIEventHandler per object");
    }
    else
    {
        AddComponentS<UIEventHandler>(ownerEntityID);
    }
}

void UIImage::SetImage(Sprite *image)
{
    _image = image;

    if (!HasComponentS<UIRenderer>(Owner))
    {
        Log::LogError("UI entity has no UI renderer attached");
        return;
    }
    auto& uii = GetComponentS<UIRenderer>(Owner);
    uii._image = _image;
    uii._isTransparent = _image->IsTransparent;

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
