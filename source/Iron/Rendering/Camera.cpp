#include "Camera.h"
#include "../Scene/Object.h"

glm::mat4 Camera::GetProjection()
{
    glm::mat4 projection = glm::ortho(-Width / 2, Width / 2, -Height / 2, Height / 2, 0.1f, Depth);
    return projection;
}

glm::mat4 Camera::GetView()
{
    glm::mat4 view = glm::mat4(1.0f);
    view = this->ParentObject->Transform->GetTransformationMatrix();
    view = glm::inverse(view);
    return view;
}
