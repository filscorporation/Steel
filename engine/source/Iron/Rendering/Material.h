#pragma once

#include <glm/glm.hpp>

#include "MaterialPropertyBlock.h"
#include "Shader.h"
#include "Sprite.h"

class Material
{
public:
    ResourceID ID = NULL_RESOURCE;
    Shader* MainShader = nullptr;
    MaterialPropertyBlock Properties;
};
