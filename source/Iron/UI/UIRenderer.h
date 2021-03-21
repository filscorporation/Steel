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
    Sprite* _image;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    QuadCache quadCache;
    bool _isTransparent = false;

    friend class UIImage;
    friend class UIButton;
};
