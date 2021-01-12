#include "Transformation.h"

glm::mat4 Transformation::GetTransformationMatrix() const
{
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(glm::mat4(1.0f), Position)
           * rotation
           * glm::scale(glm::mat4(1.0f), Scale);
}