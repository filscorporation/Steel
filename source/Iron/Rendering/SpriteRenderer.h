#pragma once

#include "../Scene/Component.h"
#include "../Core/ResourcesManager.h"

class SpriteRenderer : public Component
{
public:
    void OnRender() override;
    void SetImage(Image* image);

private:
    Image* _image;
};