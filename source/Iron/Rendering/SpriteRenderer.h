#pragma once

#include <glm/vec2.hpp>
#include "../Scene/Component.h"
#include "../Core/ResourcesManager.h"

class SpriteRenderer : public Component
{
public:
    int CurrentImageTileIndex = 0;

    void OnRender() override;
    void SetImage(Sprite* image);
    Sprite* GetImage();
    glm::vec2 GetWorldSize();

private:
    Sprite* _image;
};