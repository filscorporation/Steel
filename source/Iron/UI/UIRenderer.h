#pragma once

#include "UIComponent.h"
#include "../Rendering/Sprite.h"
#include "../Rendering/RendererData.h"

class UIRenderer : public Component
{
public:
    explicit UIRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnRender(RectTransformation& rectTransformation);
    bool IsTransparent() const;

private:
    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    QuadCache quadCache;
    bool _isTransparent = false;

    friend class UIImage;
    friend class UIButton;
};
