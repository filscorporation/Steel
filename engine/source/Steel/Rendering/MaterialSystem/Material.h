#pragma once

#include <glm/glm.hpp>

#include "MaterialPropertyBlock.h"
#include "Shader.h"
#include "Steel/Rendering/Sprite.h"

class Material
{
public:
    ResourceID ID = NULL_RESOURCE;
    Shader* MainShader = nullptr;
    MaterialPropertyBlock Properties;
};
