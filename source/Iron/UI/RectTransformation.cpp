#include "RectTransformation.h"

glm::mat4 RectTransformation::GetTransformationMatrix()
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
