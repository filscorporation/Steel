#include "Transformation.h"
#include "Entity.h"
#include "../Physics/RigidBody.h"

#define TRANSFORM_EPS 0.000001f

void Transformation::SetPosition(const glm::vec3& position)
{
    if (   std::abs(_position.x - position.x) < TRANSFORM_EPS
        && std::abs(_position.y - position.y) < TRANSFORM_EPS
        && std::abs(_position.z - position.z) < TRANSFORM_EPS)
        return;

    SetTransformationDirty(true);
    _position = position;
    UpdatePhysicsTransformation();
}

void Transformation::SetRotation(const glm::vec3& rotation)
{
    if (   std::abs(_rotation.x - rotation.x) < TRANSFORM_EPS
        && std::abs(_rotation.y - rotation.y) < TRANSFORM_EPS
        && std::abs(_rotation.z - rotation.z) < TRANSFORM_EPS)
        return;

    SetTransformationDirty(true);
    _rotation = rotation;
    UpdatePhysicsTransformation();
}

void Transformation::SetScale(const glm::vec3& scale)
{
    if (   std::abs(_scale.x - scale.x) < TRANSFORM_EPS
        && std::abs(_scale.y - scale.y) < TRANSFORM_EPS
        && std::abs(_scale.z - scale.z) < TRANSFORM_EPS)
        return;

    SetTransformationDirty(true);
    _scale = scale;
    // TODO: update colliders size
}

glm::vec3 Transformation::GetPosition() const
{
    return _position;
}

glm::vec3 Transformation::GetRotation() const
{
    return _rotation;
}

glm::vec3 Transformation::GetScale() const
{
    return _scale;
}

glm::mat4 Transformation::GetTransformationMatrix()
{
    if (IsTransformationDirty())
    {
        SetTransformationDirty(false);

        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(_rotation));
        _transformationMatrix = glm::translate(glm::mat4(1.0f), _position)
               * rotationMatrix
               * glm::scale(glm::mat4(1.0f), _scale);
    }

    return _transformationMatrix;
}

void Transformation::SetTransformationDirty(bool dirty)
{
    dirtyTransformation = dirty;
}

bool Transformation::IsTransformationDirty() const
{
    return dirtyTransformation;
}

void Transformation::UpdatePhysicsTransformation()
{
    auto rb = ParentEntity->GetComponent<RigidBody>();
    if (rb != nullptr)
    {
        rb->UpdatePhysicsTransformation();
    }
}
