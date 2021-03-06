#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Component.h"

class Transformation : public Component
{
public:
    explicit Transformation(Entity* parentEntity) : Component(parentEntity) { }

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;

    void SetTransformationDirty(bool dirty);
    bool IsTransformationDirty() const;

    glm::mat4 GetTransformationMatrix();

private:
    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };
    glm::mat4 _transformationMatrix;

    bool dirtyTransformation = true;

    void UpdatePhysicsTransformation();
};