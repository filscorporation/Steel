#pragma once

#include "../Scene/Component.h"
#include "../Core/ResourcesManager.h"

class SpriteRenderer : public Component
{
public:
    int CurrentImageTileIndex = 0;

    void OnRender() override;
    void SetImage(Sprite* image);
    Sprite* GetImage();

private:
    Sprite* _image;
};