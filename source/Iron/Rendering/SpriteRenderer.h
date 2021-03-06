#pragma once

#include <glm/vec2.hpp>
#include "../Scene/Component.h"
#include "../Core/ResourcesManager.h"

class SpriteRenderer : public Component
{
public:
    explicit SpriteRenderer(Entity* parentEntity) : Component(parentEntity) { }

    int CurrentImageTileIndex = 0;

    void OnRender();
    void SetImage(Sprite* image);
    Sprite* GetImage();
    glm::vec2 GetWorldSize();

private:
    Sprite* _image;
};