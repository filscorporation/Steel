#include "Camera.h"
#include "../Scene/Object.h"

void Camera::SetWidth(float width)
{
    SetCameraDirty(true);
    _width = width;
    _height = _width * (float)Screen::GetHeight() / (float)Screen::GetWidth();
}

void Camera::SetHeight(float height)
{
    SetCameraDirty(true);
    _height = height;
    _width = _height * (float)Screen::GetWidth() / (float)Screen::GetHeight();
}

void Camera::SetSize(float width, float height)
{
    SetCameraDirty(true);
    _width = width;
    _height = height;
}

void Camera::SetDepth(float depth)
{
    SetCameraDirty(true);
    _depth = depth;
}

float Camera::GetWidth() const
{
    return _width;
}

float Camera::GetHeight() const
{
    return _height;
}

float Camera::GetDepth() const
{
    return _depth;
}

glm::mat4 Camera::GetViewProjection()
{
    if (IsCameraDirty())
    {
        SetCameraDirty(false);
        glm::mat4 projection = glm::ortho(-_width / 2, _width / 2, -_height / 2, _height / 2, 0.1f, _depth);
        glm::mat4 view = this->ParentObject->Transform->GetTransformationMatrix();
        view = glm::inverse(view);

        viewProjection = projection * view;
    }

    return viewProjection;
}

void Camera::SetCameraDirty(bool dirty)
{
    dirtyCamera = dirty;
}

bool Camera::IsCameraDirty() const
{
    return dirtyCamera;
}
