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

    virtual glm::vec3 GetPosition() const;
    virtual void SetPosition(const glm::vec3& position);
    glm::vec3 GetRotation() const;
    void SetRotation(const glm::vec3& rotation);
    glm::vec3 GetScale() const;
    void SetScale(const glm::vec3& scale);

    void SetTransformationDirty(bool dirty);
    bool IsTransformationDirty() const;

    virtual glm::mat4 GetTransformationMatrix();

protected:
    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };

    bool dirtyTransformation = true;

    virtual void UpdatePhysicsTransformation();

private:
    glm::mat4 _transformationMatrix;
};