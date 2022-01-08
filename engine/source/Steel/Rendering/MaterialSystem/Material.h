#pragma once

#include <glm/glm.hpp>

#include "MaterialPropertyBlock.h"
#include "Shader.h"
#include "Steel/Rendering/Sprite.h"

class Material : public Resource
{
public:
    Material();

    Shader* MainShader = nullptr;
    MaterialPropertyBlock Properties;
};
