#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "../Core/ResourceID.h"
#include "../Physics/RigidBody.h"
#include "../Rendering/Camera.h"

namespace ComponentsInternalCalls
{
    // Transformation
    glm::vec3 Transformation_GetPosition(EntityID entityID);
    void Transformation_SetPosition(EntityID entityID, glm::vec3 position);
    glm::vec3 Transformation_GetRotation(EntityID entityID);
    void Transformation_SetRotation(EntityID entityID, glm::vec3 rotation);
    glm::vec3 Transformation_GetScale(EntityID entityID);
    void Transformation_SetScale(EntityID entityID, glm::vec3 scale);

    // Audio
    float AudioListener_GetVolume(EntityID entityID);
    void AudioListener_SetVolume(EntityID entityID, float volume);
    float AudioSource_GetVolume(EntityID entityID);
    void AudioSource_SetVolume(EntityID entityID, float volume);
    bool AudioSource_GetLoop(EntityID entityID);
    void AudioSource_SetLoop(EntityID entityID, bool loop);
    void AudioSource_Play(EntityID entityID, ResourceID audioTrackID);
    void AudioSource_Stop(EntityID entityID);

    // Sprite Renderer
    ResourceID SpriteRenderer_GetSprite(EntityID entityID);
    void SpriteRenderer_SetSprite(EntityID entityID, ResourceID spriteID);

    // Animator
    void Animator_PlayAnimation(EntityID entityID, ResourceID animationID);
    void Animator_Play(EntityID entityID);
    void Animator_Pause(EntityID entityID);
    void Animator_Stop(EntityID entityID);
    void Animator_Restart(EntityID entityID);

    // Camera
    EntityID Camera_GetEntityWithMainCamera();
    float Camera_GetWidth(EntityID entityID);
    void Camera_SetWidth(EntityID entityID, float width);
    float Camera_GetHeight(EntityID entityID);
    void Camera_SetHeight(EntityID entityID, float height);
    float Camera_GetNearClippingPlane(EntityID entityID);
    void Camera_SetNearClippingPlane(EntityID entityID, float distance);
    float Camera_GetFarClippingPlane(EntityID entityID);
    void Camera_SetFarClippingPlane(EntityID entityID, float distance);
    CameraResizeModes::CameraResizeMode Camera_GetResizingMode(EntityID entityID);
    void Camera_SetResizingMode(EntityID entityID, CameraResizeModes::CameraResizeMode mode);
    void Camera_WorldToScreenPoint(EntityID entityID, glm::vec2* worldPoint, glm::vec2* screenPoint);
    void Camera_ScreenToWorldPoint(EntityID entityID, glm::vec2* screenPoint, glm::vec2* worldPoint);

    // Physics
    float RigidBody_GetMass(EntityID entityID);
    void RigidBody_SetMass(EntityID entityID, float mass);
    RigidBodyTypes::RigidBodyType RigidBody_GetRigidBodyType(EntityID entityID);
    void RigidBody_SetRigidBodyType(EntityID entityID, RigidBodyTypes::RigidBodyType type);
    void BoxCollider_GetSize(EntityID entityID, glm::vec2* size);
    void BoxCollider_SetSize(EntityID entityID, glm::vec2* size);
    float CircleCollider_GetRadius(EntityID entityID);
    void CircleCollider_SetRadius(EntityID entityID, float radius);
}
