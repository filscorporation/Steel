#pragma once

#include "../Scene/Component.h"
#include <glm/glm.hpp>

class Camera : public Component
{
public:
    void SetWidth(float width);
    void SetHeight(float height);
    void SetSize(float width, float height);
    void SetDepth(float depth);
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;

    glm::mat4 GetViewProjection();

private:
    float _width = 4.0f;
    float _height = 3.0f;
    float _depth = 10.0f;

    glm::mat4 viewProjection;

    void SetCameraDirty(bool dirty);
    bool IsCameraDirty() const;
    bool dirtyCamera = true;
};