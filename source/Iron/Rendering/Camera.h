#pragma once

#include "../Scene/Component.h"
#include <glm/glm.hpp>

namespace CameraResizeModes
{
    enum CameraResizeMode
    {
        KeepHeight,
        KeepWidth,
        Stretch,
    };
}

class Camera : public Component
{
public:
    explicit Camera(Entity* parentEntity) : Component(parentEntity) { }

    float GetWidth() const;
    void SetWidth(float width);
    float GetHeight() const;
    void SetHeight(float height);
    float GetNearClippingPlane() const;
    void SetNearClippingPlane(float distance);
    float GetFarClippingPlane() const;
    void SetFarClippingPlane(float distance);
    CameraResizeModes::CameraResizeMode GetResizeMode();
    void SetResizeMode(CameraResizeModes::CameraResizeMode resizeMode);
    void UpdateSize();

    glm::vec2 ScreenToWorldPoint(glm::vec2 screenPoint);
    glm::vec2 WorldToScreenPoint(glm::vec2 worldPoint);

    glm::mat4 GetViewProjection();

private:
    float _width = 4.0f;
    float _height = 3.0f;
    float _nearClippingPlane = 0.1f;
    float _farClippingPlane = 10.0f;
    CameraResizeModes::CameraResizeMode _resizeMode = CameraResizeModes::KeepHeight;

    glm::mat4 viewProjection;

    void SetCameraDirty(bool dirty);
    bool IsCameraDirty() const;
    bool dirtyCamera = true;
};