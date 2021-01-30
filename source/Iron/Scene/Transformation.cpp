#include "Transformation.h"

void Transformation::SetPosition(const glm::vec3& position)
{
    SetTransformationDirty(true);
    _position = position;
}

void Transformation::SetRotation(const glm::vec3& rotation)
{
    SetTransformationDirty(true);
    _rotation = rotation;
}

void Transformation::SetScale(const glm::vec3& scale)
{
    SetTransformationDirty(true);
    _scale = scale;
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
