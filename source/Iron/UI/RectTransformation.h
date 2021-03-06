#pragma once

#include "../Scene/Transformation.h"

class RectTransformation : public Transformation
{
public:
    explicit RectTransformation(Entity* parentEntity) : Transformation(parentEntity) { }

    glm::mat4 GetTransformationMatrix() override;

private:
    glm::mat4 _transformationMatrix;
};
