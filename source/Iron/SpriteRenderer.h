#pragma once

#include "Component.h"

class SpriteRenderer : public Component
{
public:
    const char* SpriteName = nullptr;

    SpriteRenderer();
    ~SpriteRenderer();
    void OnRender(Camera* camera) override;
};