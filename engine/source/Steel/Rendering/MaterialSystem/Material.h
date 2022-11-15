#pragma once

#include <glm/glm.hpp>

#include "MaterialPropertyBlock.h"
#include "Shader.h"
#include "Steel/Rendering/Sprite.h"

class Material : public Resource
{
    DEFINE_TYPE(Material)

public:
    Material();

    Shader* MainShader = nullptr;
    MaterialPropertyBlock Properties;
};
