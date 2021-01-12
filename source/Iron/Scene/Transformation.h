#pragma once

#include <GLM/vec3.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>

#include "Component.h"
#include "../Core/Input.h"

class Transformation : public Component
{
public:
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

    glm::mat4 GetTransformationMatrix() const;
};