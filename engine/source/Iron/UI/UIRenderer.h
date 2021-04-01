#pragma once

#include "UIComponent.h"
#include "../Rendering/Sprite.h"
#include "../Rendering/RendererData.h"

class UIRenderer : public Component
{
public:
    explicit UIRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void Render(RectTransformation& rectTransformation);

    bool IsRendered = true;
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned int TextureID = 0;
    glm::vec2 TextureCoords[4] = { glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                   glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
    bool IsTransparent = false;

    QuadCache quadCache;
};
