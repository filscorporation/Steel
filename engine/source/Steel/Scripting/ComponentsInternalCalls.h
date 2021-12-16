#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <mono/jit/jit.h>

#include "Steel/Physics/RigidBody.h"
#include "Steel/Rendering/Camera.h"
#include "Steel/Resources/ResourceID.h"
#include "Steel/Scripting/InternalStructures/MaterialPropertyBlockInternal.h"
#include "Steel/Scripting/InternalStructures/MeshInternal.h"

namespace ComponentsInternalCalls
{
    // Transformation
    glm::vec3 Transformation_GetPosition(EntityID entityID);
    void Transformation_SetPosition(EntityID entityID, glm::vec3 position);
    glm::vec3 Transformation_GetLocalPosition(EntityID entityID);
    void Transformation_SetLocalPosition(EntityID entityID, glm::vec3 position);
    glm::vec3 Transformation_GetRotation(EntityID entityID);
    void Transformation_SetRotation(EntityID entityID, glm::vec3 rotation);
    glm::vec3 Transformation_GetLocalRotation(EntityID entityID);
    void Transformation_SetLocalRotation(EntityID entityID, glm::vec3 rotation);
    glm::vec3 Transformation_GetScale(EntityID entityID);
    void Transformation_SetScale(EntityID entityID, glm::vec3 scale);
    glm::vec3 Transformation_GetLocalScale(EntityID entityID);
    void Transformation_SetLocalScale(EntityID entityID, glm::vec3 scale);

    // Hierarchy Node
    EntityID HierarchyNode_GetParent(EntityID entityID);
    void HierarchyNode_SetParent(EntityID entityID, EntityID parentEntityID);
    MonoArray* HierarchyNode_GetChildren(EntityID entityID);

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
    ResourceID SpriteRenderer_GetMaterial(EntityID entityID);
    void SpriteRenderer_SetMaterial(EntityID entityID, ResourceID materialID);
    void SpriteRenderer_GetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal* properties);
    void SpriteRenderer_SetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal properties);

    // Mesh Renderer
    void MeshRenderer_GetMesh(EntityID entityID, MeshInternal* mesh);
    void MeshRenderer_SetMesh(EntityID entityID, MeshInternal mesh);
    ResourceID MeshRenderer_GetMaterial(EntityID entityID);
    void MeshRenderer_SetMaterial(EntityID entityID, ResourceID materialID);
    void MeshRenderer_GetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal* properties);
    void MeshRenderer_SetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal properties);

    // Animator
    void Animator_PlayAnimation(EntityID entityID, ResourceID animationID);
    void Animator_PlayAnimation2(EntityID entityID, MonoString* animationName);
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
    RigidBodyTypes::RigidBodyType RigidBody_GetRigidBodyType(EntityID entityID);
    void RigidBody_SetRigidBodyType(EntityID entityID, RigidBodyTypes::RigidBodyType type);
    float RigidBody_GetMass(EntityID entityID);
    void RigidBody_SetMass(EntityID entityID, float mass);
    void RigidBody_GetVelocity(EntityID entityID, glm::vec2* velocity);
    void RigidBody_SetVelocity(EntityID entityID, glm::vec2* velocity);
    float RigidBody_GetAngularVelocity(EntityID entityID);
    void RigidBody_SetAngularVelocity(EntityID entityID, float velocity);
    float RigidBody_GetGravityScale(EntityID entityID);
    void RigidBody_SetGravityScale(EntityID entityID, float gravityScale);
    float RigidBody_GetFriction(EntityID entityID);
    void RigidBody_SetFriction(EntityID entityID, float friction);
    float RigidBody_GetRestitution(EntityID entityID);
    void RigidBody_SetRestitution(EntityID entityID, float restitution);
    float RigidBody_GetLinearDamping(EntityID entityID);
    void RigidBody_SetLinearDamping(EntityID entityID, float damping);
    float RigidBody_GetAngularDamping(EntityID entityID);
    void RigidBody_SetAngularDamping(EntityID entityID, float damping);
    bool RigidBody_GetFixedRotation(EntityID entityID);
    void RigidBody_SetFixedRotation(EntityID entityID, float isFixed);
    bool RigidBody_GetUseContinuousCollisionDetection(EntityID entityID);
    void RigidBody_SetUseContinuousCollisionDetection(EntityID entityID, bool useCCD);
    void RigidBody_ApplyForce(EntityID entityID, glm::vec2* force);
    void RigidBody_ApplyForce2(EntityID entityID, glm::vec2* force, glm::vec2* point);
    void RigidBody_ApplyTorque(EntityID entityID, float torque);
    void RigidBody_ApplyLinearImpulse(EntityID entityID, glm::vec2* impulse);
    void RigidBody_ApplyLinearImpulse2(EntityID entityID, glm::vec2* impulse, glm::vec2* point);
    void RigidBody_ApplyAngularImpulse(EntityID entityID, float impulse);

    void BoxCollider_GetSize(EntityID entityID, glm::vec2* size);
    void BoxCollider_SetSize(EntityID entityID, glm::vec2* size);
    float CircleCollider_GetRadius(EntityID entityID);
    void CircleCollider_SetRadius(EntityID entityID, float radius);
}
