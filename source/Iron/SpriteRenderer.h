#pragma once

#include "Component.h"

class SpriteRenderer : Component
{
public:
    const char* SpriteName = nullptr;

    SpriteRenderer();
    ~SpriteRenderer();
    void OnRender() override;
};