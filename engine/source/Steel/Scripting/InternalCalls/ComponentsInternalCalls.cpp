#include "ComponentsInternalCalls.h"
#include "InternalCallsCommon.h"
#include "Steel/Animation/Animator.h"
#include "Steel/Audio/AudioSource.h"
#include "Steel/Audio/AudioListener.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Physics/BoxCollider.h"
#include "Steel/Physics/CircleCollider.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/Rendering/MeshRenderer.h"
#include "Steel/Rendering/Camera.h"
#include "Steel/Scene/Hierarchy.h"
#include "Steel/Scene/HierarchyNode.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scripting/ScriptingCore.h"

glm::vec3 ComponentsInternalCalls::Transformation_GetPosition(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetPosition();
}

void ComponentsInternalCalls::Transformation_SetPosition(EntityID entityID, glm::vec3 position)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetPosition(position);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetLocalPosition(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetLocalPosition();
}

void ComponentsInternalCalls::Transformation_SetLocalPosition(EntityID entityID, glm::vec3 position)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetLocalPosition(position);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetRotation(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetRotation();
}

void ComponentsInternalCalls::Transformation_SetRotation(EntityID entityID, glm::vec3 rotation)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetRotation(rotation);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetLocalRotation(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetLocalRotation();
}

void ComponentsInternalCalls::Transformation_SetLocalRotation(EntityID entityID, glm::vec3 rotation)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetLocalRotation(rotation);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetScale(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetScale();
}

void ComponentsInternalCalls::Transformation_SetScale(EntityID entityID, glm::vec3 scale)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetScale(scale);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetLocalScale(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Transformation, glm::vec3(0.0f))

    return component.GetLocalScale();
}

void ComponentsInternalCalls::Transformation_SetLocalScale(EntityID entityID, glm::vec3 scale)
{
    GET_COMPONENT_OR_RETURN(Transformation, )

    component.SetLocalScale(scale);
}

EntityID ComponentsInternalCalls::HierarchyNode_GetParent(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(HierarchyNode, NULL_ENTITY)

    return component.GetParentNode();
}

void ComponentsInternalCalls::HierarchyNode_SetParent(EntityID entityID, EntityID parentEntityID)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    LinkChildToParent(registry, entityID, parentEntityID);
}

MonoArray* ComponentsInternalCalls::HierarchyNode_GetChildren(EntityID entityID)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    return ScriptingCore::ToMonoUInt32Array(GetAllChildren(registry, entityID));
}

float ComponentsInternalCalls::AudioListener_GetVolume(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(AudioListener, 0.0f)

    return component.GetVolume();
}

void ComponentsInternalCalls::AudioListener_SetVolume(EntityID entityID, float volume)
{
    GET_COMPONENT_OR_RETURN(AudioListener, )

    component.SetVolume(volume);
}

float ComponentsInternalCalls::AudioSource_GetVolume(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(AudioSource, 0.0f)

    return component.GetVolume();
}

void ComponentsInternalCalls::AudioSource_SetVolume(EntityID entityID, float volume)
{
    GET_COMPONENT_OR_RETURN(AudioSource, )

    component.SetVolume(volume);
}

bool ComponentsInternalCalls::AudioSource_GetLoop(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(AudioSource, false)

    return component.GetIsLoop();
}

void ComponentsInternalCalls::AudioSource_SetLoop(EntityID entityID, bool loop)
{
    GET_COMPONENT_OR_RETURN(AudioSource, )

    component.SetIsLoop(loop);
}

void ComponentsInternalCalls::AudioSource_Play(EntityID entityID, ResourceID audioTrackID)
{
    GET_COMPONENT_OR_RETURN(AudioSource, )

    component.Play(Application::Instance->GetResourcesManager()->GetAudioTrack(audioTrackID));
}

void ComponentsInternalCalls::AudioSource_Stop(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(AudioSource, )

    component.Stop();
}

ResourceID ComponentsInternalCalls::SpriteRenderer_GetSprite(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, NULL_RESOURCE)
    auto image = component.GetImage();

    return image == nullptr ? 0 : image->ID;
}

void ComponentsInternalCalls::SpriteRenderer_SetSprite(EntityID entityID, ResourceID spriteID)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, )

    component.SetImage(Application::Instance->GetResourcesManager()->GetSprite(spriteID));
}

ResourceID ComponentsInternalCalls::SpriteRenderer_GetMaterial(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, NULL_RESOURCE)
    auto material = component.GetMaterial();

    return material == nullptr ? 0 : material->ID;
}

void ComponentsInternalCalls::SpriteRenderer_SetMaterial(EntityID entityID, ResourceID materialID)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, )

    component.SetMaterial(Application::Instance->GetResourcesManager()->GetMaterial(materialID));
}

void ComponentsInternalCalls::SpriteRenderer_GetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal* properties)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, )

    properties->FromMaterialPropertyBlock(component.GetCustomProperties());
}

void ComponentsInternalCalls::SpriteRenderer_SetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal properties)
{
    GET_COMPONENT_OR_RETURN(SpriteRenderer, )

    MaterialPropertyBlock propertiesOut;
    properties.ToMaterialPropertyBlock(propertiesOut);
    component.SetCustomProperties(propertiesOut);
}

void ComponentsInternalCalls::MeshRenderer_GetMesh(EntityID entityID, MeshInternal* mesh)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, )

    mesh->FromMesh(component.GetMesh());
}

void ComponentsInternalCalls::MeshRenderer_SetMesh(EntityID entityID, MeshInternal mesh)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, )

    Mesh outMesh;
    mesh.ToMesh(outMesh);
    component.SetMesh(outMesh);
}

ResourceID ComponentsInternalCalls::MeshRenderer_GetMaterial(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, NULL_RESOURCE)
    auto material = component.GetMaterial();

    return material == nullptr ? 0 : material->ID;
}

void ComponentsInternalCalls::MeshRenderer_SetMaterial(EntityID entityID, ResourceID materialID)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, )

    component.SetMaterial(Application::Instance->GetResourcesManager()->GetMaterial(materialID));
}

void ComponentsInternalCalls::MeshRenderer_GetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal* properties)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, )

    properties->FromMaterialPropertyBlock(component.GetCustomProperties());
}

void ComponentsInternalCalls::MeshRenderer_SetCustomMaterialProperties(EntityID entityID, MaterialPropertyBlockInternal properties)
{
    GET_COMPONENT_OR_RETURN(MeshRenderer, )

    MaterialPropertyBlock propertiesOut;
    properties.ToMaterialPropertyBlock(propertiesOut);
    component.SetCustomProperties(propertiesOut);
}

void ComponentsInternalCalls::Animator_PlayAnimation(EntityID entityID, ResourceID animationID)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
    {
        Log::LogError("Animation does not exist");
        return;
    }

    component.AddAndPlay(animation);
}

void ComponentsInternalCalls::Animator_PlayAnimation2(EntityID entityID, MonoString* animationName)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    component.Play(ScriptingCore::ToString(animationName));
}

void ComponentsInternalCalls::Animator_Play(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    component.Play();
}

void ComponentsInternalCalls::Animator_Pause(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    component.Pause();
}

void ComponentsInternalCalls::Animator_Stop(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    component.Stop();
}

void ComponentsInternalCalls::Animator_Restart(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    component.Restart();
}

void ComponentsInternalCalls::Animator_AddAnimations(EntityID entityID, MonoArray* animationIDs)
{
    GET_COMPONENT_OR_RETURN(Animator, )

    std::vector<ResourceID> ids;
    ScriptingCore::FromMonoUInt64Array(animationIDs, ids);

    std::vector<Animation*> animations;
    animations.reserve(ids.size());
    for (auto id : ids)
    {
        animations.push_back(Application::Instance->GetResourcesManager()->GetAnimation(id));
    }

    component.AddAnimations(animations);
}

EntityID ComponentsInternalCalls::Camera_GetEntityWithMainCamera()
{
    return Application::Instance->GetCurrentScene()->GetMainCamera();
}

float ComponentsInternalCalls::Camera_GetWidth(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Camera, 0.0f)

    return component.GetWidth();
}

void ComponentsInternalCalls::Camera_SetWidth(EntityID entityID, float width)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    component.SetWidth(width);
}

float ComponentsInternalCalls::Camera_GetHeight(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Camera, 0.0f)

    return component.GetHeight();
}

void ComponentsInternalCalls::Camera_SetHeight(EntityID entityID, float height)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    component.SetHeight(height);
}

float ComponentsInternalCalls::Camera_GetNearClippingPlane(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Camera, 0.0f)

    return component.GetNearClippingPlane();
}

void ComponentsInternalCalls::Camera_SetNearClippingPlane(EntityID entityID, float distance)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    component.SetNearClippingPlane(distance);
}

float ComponentsInternalCalls::Camera_GetFarClippingPlane(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Camera, 0.0f)

    return component.GetFarClippingPlane();
}

void ComponentsInternalCalls::Camera_SetFarClippingPlane(EntityID entityID, float distance)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    component.SetFarClippingPlane(distance);
}

CameraResizeModes::CameraResizeMode ComponentsInternalCalls::Camera_GetResizingMode(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(Camera, (CameraResizeModes::CameraResizeMode)0)

    return component.GetResizeMode();
}

void ComponentsInternalCalls::Camera_SetResizingMode(EntityID entityID, CameraResizeModes::CameraResizeMode mode)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    component.SetResizeMode(mode);
}

void ComponentsInternalCalls::Camera_WorldToScreenPoint(EntityID entityID, glm::vec2* worldPoint, glm::vec2* screenPoint)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    glm::vec2 result = component.WorldToScreenPoint(*worldPoint);
    screenPoint->x = result.x;
    screenPoint->y = result.y;
}

void ComponentsInternalCalls::Camera_ScreenToWorldPoint(EntityID entityID, glm::vec2* screenPoint, glm::vec2* worldPoint)
{
    GET_COMPONENT_OR_RETURN(Camera, )

    glm::vec2 result = component.ScreenToWorldPoint(*screenPoint);
    worldPoint->x = result.x;
    worldPoint->y = result.y;
}

RigidBodyTypes::RigidBodyType ComponentsInternalCalls::RigidBody_GetRigidBodyType(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, (RigidBodyTypes::RigidBodyType)0)

    return component.GetType();
}

void ComponentsInternalCalls::RigidBody_SetRigidBodyType(EntityID entityID, RigidBodyTypes::RigidBodyType type)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetType(type);
}

float ComponentsInternalCalls::RigidBody_GetMass(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetMass();
}

void ComponentsInternalCalls::RigidBody_SetMass(EntityID entityID, float mass)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetMass(mass);
}

void ComponentsInternalCalls::RigidBody_GetVelocity(EntityID entityID, glm::vec2* velocity)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    glm::vec2 result = component.GetVelocity();
    velocity->x = result.x;
    velocity->y = result.y;
}

void ComponentsInternalCalls::RigidBody_SetVelocity(EntityID entityID, glm::vec2* velocity)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetVelocity(glm::vec2(velocity->x, velocity->y));
}

float ComponentsInternalCalls::RigidBody_GetAngularVelocity(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetAngularVelocity();
}

void ComponentsInternalCalls::RigidBody_SetAngularVelocity(EntityID entityID, float velocity)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetAngularVelocity(velocity);
}

float ComponentsInternalCalls::RigidBody_GetGravityScale(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetGravityScale();
}

void ComponentsInternalCalls::RigidBody_SetGravityScale(EntityID entityID, float gravityScale)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetGravityScale(gravityScale);
}

float ComponentsInternalCalls::RigidBody_GetFriction(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetFriction();
}

void ComponentsInternalCalls::RigidBody_SetFriction(EntityID entityID, float friction)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetFriction(friction);
}

float ComponentsInternalCalls::RigidBody_GetRestitution(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetRestitution();
}

void ComponentsInternalCalls::RigidBody_SetRestitution(EntityID entityID, float restitution)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetRestitution(restitution);
}

float ComponentsInternalCalls::RigidBody_GetLinearDamping(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetLinearDamping();
}

void ComponentsInternalCalls::RigidBody_SetLinearDamping(EntityID entityID, float damping)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetLinearDamping(damping);
}

float ComponentsInternalCalls::RigidBody_GetAngularDamping(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, 0.0f)

    return component.GetAngularDamping();
}

void ComponentsInternalCalls::RigidBody_SetAngularDamping(EntityID entityID, float damping)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetAngularDamping(damping);
}

bool ComponentsInternalCalls::RigidBody_GetFixedRotation(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, false)

    return component.GetIsFixedRotation();
}

void ComponentsInternalCalls::RigidBody_SetFixedRotation(EntityID entityID, float isFixed)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetIsFixedRotation(isFixed);
}

bool ComponentsInternalCalls::RigidBody_GetUseContinuousCollisionDetection(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RigidBody, false)

    return component.GetUseContinuousCollisionDetection();
}

void ComponentsInternalCalls::RigidBody_SetUseContinuousCollisionDetection(EntityID entityID, bool useCCD)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.SetUseContinuousCollisionDetection(useCCD);
}

void ComponentsInternalCalls::RigidBody_ApplyForce(EntityID entityID, glm::vec2* force)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyForce(*force);
}

void ComponentsInternalCalls::RigidBody_ApplyForce2(EntityID entityID, glm::vec2* force, glm::vec2* point)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyForce(*force, *point);
}

void ComponentsInternalCalls::RigidBody_ApplyTorque(EntityID entityID, float torque)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyTorque(torque);
}

void ComponentsInternalCalls::RigidBody_ApplyLinearImpulse(EntityID entityID, glm::vec2* impulse)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyLinearImpulse(*impulse);
}

void ComponentsInternalCalls::RigidBody_ApplyLinearImpulse2(EntityID entityID, glm::vec2* impulse, glm::vec2* point)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyLinearImpulse(*impulse, *point);
}

void ComponentsInternalCalls::RigidBody_ApplyAngularImpulse(EntityID entityID, float impulse)
{
    GET_COMPONENT_OR_RETURN(RigidBody, )

    component.ApplyAngularImpulse(impulse);
}

void ComponentsInternalCalls::BoxCollider_GetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(BoxCollider, )

    auto s = component.GetSize();
    size->x = s.x;
    size->y = s.y;
}

void ComponentsInternalCalls::BoxCollider_SetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(BoxCollider, )

    component.SetSize(*size);
}

float ComponentsInternalCalls::CircleCollider_GetRadius(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(CircleCollider, 0.0f)

    return component.GetRadius();
}

void ComponentsInternalCalls::CircleCollider_SetRadius(EntityID entityID, float radius)
{
    GET_COMPONENT_OR_RETURN(CircleCollider, )

    component.SetRadius(radius);
}
