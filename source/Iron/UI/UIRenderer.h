#pragma once

#include "UIComponent.h"
#include "../Rendering/Sprite.h"
#include "../Rendering/RendererData.h"

class UIRenderer : public UIComponent
{
public:
    explicit UIRenderer(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    void OnRender(RectTransformation& rectTransformation);
    bool IsTransparent() const;

private:
    Sprite* _image; // TODO: UI data
    QuadCache quadCache;
    bool _isTransparent = false;

    friend class UIImage;
};
