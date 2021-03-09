#pragma once

#include <glm/vec2.hpp>

#include "Sprite.h"
#include "../EntityComponentSystem/Component.h"

class SpriteRenderer : public Component
{
public:
    explicit SpriteRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    int CurrentImageTileIndex = 0;

    void OnRender();
    void SetImage(Sprite* image);
    Sprite* GetImage();
    glm::vec2 GetWorldSize();

private:
    Sprite* _image;
};
