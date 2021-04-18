#pragma once

#include <glm/vec2.hpp>

#include "Sprite.h"
#include "RendererData.h"
#include "../EntityComponentSystem/Component.h"
#include "../Scene/Transformation.h"

class SpriteRenderer : public Component
{
public:
    explicit SpriteRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void UpdateRenderer(Transformation& transformation, bool transformationDirty);

    void SetImage(Sprite* image);
    Sprite* GetImage();
    void SetTileIndex(uint32_t tileIndex);
    uint32_t GetTileIndex() const;
    glm::vec2 GetWorldSize();
    bool IsTransparent() const;

private:
    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t currentImageTileIndex = 0;
    bool _isTransparent = false;
    bool imageDirty = true;
};
