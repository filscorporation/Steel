#pragma once

#include "../EntityComponentSystem/Component.h"
#include "../Scene/Transformation.h"

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
    COMPONENT(Camera)

public:
    explicit Camera(EntityID ownerEntityID) : Component(ownerEntityID) { }

    static void RegisterType();

    float GetWidth() const;
    void SetWidth(float width);
    float GetHeight() const;
    void SetHeight(float height);
    float GetNearClippingPlane() const;
    void SetNearClippingPlane(float distance);
    float GetFarClippingPlane() const;
    void SetFarClippingPlane(float distance);
    CameraResizeModes::CameraResizeMode GetResizeMode() const;
    void SetResizeMode(CameraResizeModes::CameraResizeMode resizeMode);

    glm::vec2 ScreenToWorldPoint(glm::vec2 screenPoint);
    glm::vec2 WorldToScreenPoint(glm::vec2 worldPoint);

    void UpdateSize(float screenWidth, float screenHeight);
    void UpdateViewProjection(Transformation& transformation);
    glm::mat4 GetViewProjection();

    bool AutoResize = true; // False used for scene camera

private:
    float _width = 4.0f;
    float _height = 3.0f;
    float _nearClippingPlane = 0.1f;
    float _farClippingPlane = 100.0f;
    CameraResizeModes::CameraResizeMode _resizeMode = CameraResizeModes::KeepHeight;

    glm::mat4 viewProjection;

    void SetCameraDirty(bool dirty);
    bool IsCameraDirty() const;
    bool dirtyCamera = true;
};
