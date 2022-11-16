#pragma once

#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/Rendering/Framebuffer.h"
#include "Steel/Rendering/Core/RendererEnums.h"
#include "Steel/Scene/Transformation.h"

#include <glm/glm.hpp>

typedef uint32_t RenderMask;

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
    DEFINE_TYPE(Camera)

public:
    explicit Camera(EntityID ownerEntityID) : Component(ownerEntityID) { }

    RenderMask GetRenderMask() const;
    void SetRenderMask(RenderMask mask);
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
    ClearFlags::ClearFlag GetClearFlag() const;
    void SetClearFlag(ClearFlags::ClearFlag clearFlag);
    Framebuffer* GetCustomTargetFramebuffer() const;
    void SetCustomTargetFramebuffer(Framebuffer* framebuffer);

    glm::vec2 ScreenToWorldPoint(glm::vec2 screenPoint);
    glm::vec2 WorldToScreenPoint(glm::vec2 worldPoint);

    void UpdateSize(float screenWidth, float screenHeight);
    void UpdateViewProjection(Transformation& transformation);
    glm::mat4 GetViewProjection();

    bool AutoResize = true; // False used for scene camera

private:
    RenderMask _mask = (RenderMask)-1; // All
    float _width = 4.0f;
    float _height = 3.0f;
    float _nearClippingPlane = 0.1f;
    float _farClippingPlane = 100.0f;
    CameraResizeModes::CameraResizeMode _resizeMode = CameraResizeModes::KeepHeight;
    ClearFlags::ClearFlag _clearFlag = ClearFlags::All;
    Framebuffer* _customTargetFramebuffer = nullptr; // Null for default framebuffer (TODO: maybe replace with texture)

    glm::mat4 viewProjection;

    void SetCameraDirty(bool dirty);
    bool IsCameraDirty() const;
    bool dirtyCamera = true;
};
