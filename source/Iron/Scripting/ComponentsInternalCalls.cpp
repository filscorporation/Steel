#include "ComponentsInternalCalls.h"
#include "../Animation/Animator.h"
#include "../Audio/AudioSource.h"
#include "../Audio/AudioListener.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/Camera.h"
#include "../Physics/BoxCollider.h"

glm::vec3 ComponentsInternalCalls::Transformation_GetPosition(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return glm::vec3();
    }

    return entity->Transform->GetPosition();
}

void ComponentsInternalCalls::Transformation_SetPosition(int64_t entityID, glm::vec3 position)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    entity->Transform->SetPosition(position);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetRotation(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return glm::vec3();
    }

    return entity->Transform->GetRotation();
}

void ComponentsInternalCalls::Transformation_SetRotation(int64_t entityID, glm::vec3 rotation)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    entity->Transform->SetRotation(rotation);
}

glm::vec3 ComponentsInternalCalls::Transformation_GetScale(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return glm::vec3();
    }

    return entity->Transform->GetScale();
}

void ComponentsInternalCalls::Transformation_SetScale(int64_t entityID, glm::vec3 scale)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    entity->Transform->SetScale(scale);
}

float ComponentsInternalCalls::AudioListener_GetVolume(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto al = entity->GetComponent<AudioListener>();
    if (al == nullptr)
    {
        Log::LogError("No audio listener component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return al->GetVolume();
}

void ComponentsInternalCalls::AudioListener_SetVolume(int64_t entityID, float volume)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto al = entity->GetComponent<AudioListener>();
    if (al == nullptr)
    {
        Log::LogError("No audio listener component attached to entity " + std::to_string(entityID));
        return;
    }

    al->SetVolume(volume);
}

float ComponentsInternalCalls::AudioSource_GetVolume(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return as->GetVolume();
}

void ComponentsInternalCalls::AudioSource_SetVolume(int64_t entityID, float volume)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return;
    }

    as->SetVolume(volume);
}

bool ComponentsInternalCalls::AudioSource_GetLoop(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return false;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return false;
    }

    return as->GetIsLoop();
}

void ComponentsInternalCalls::AudioSource_SetLoop(int64_t entityID, bool loop)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return;
    }

    as->SetIsLoop(loop);
}

void ComponentsInternalCalls::AudioSource_Play(int64_t entityID, int64_t audioTrackID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return;
    }

    as->Play(Application::Instance->GetResourcesManager()->GetAudioTrack(audioTrackID));
}

void ComponentsInternalCalls::AudioSource_Stop(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto as = entity->GetComponent<AudioSource>();
    if (as == nullptr)
    {
        Log::LogError("No audio source component attached to entity " + std::to_string(entityID));
        return;
    }

    as->Stop();
}

uint64_t ComponentsInternalCalls::SpriteRenderer_GetSprite(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto sr = entity->GetComponent<SpriteRenderer>();
    if (sr == nullptr)
    {
        Log::LogError("No sprite renderer component attached to entity " + std::to_string(entityID));
        return 0;
    }

    auto image = sr->GetImage();

    return image == nullptr ? 0 : image->ID;
}

void ComponentsInternalCalls::SpriteRenderer_SetSprite(int64_t entityID, int64_t spriteID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto sr = entity->GetComponent<SpriteRenderer>();
    if (sr == nullptr)
    {
        Log::LogError("No sprite renderer component attached to entity " + std::to_string(entityID));
        return;
    }

    sr->SetImage(Application::Instance->GetResourcesManager()->GetImage(spriteID));
}

void ComponentsInternalCalls::Animator_PlayAnimation(int64_t entityID, int64_t animationID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto animator = entity->GetComponent<Animator>();
    if (animator == nullptr)
    {
        Log::LogError("No animator component attached to entity " + std::to_string(entityID));
        return;
    }

    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
    {
        Log::LogError("Animation does not exist");
        return;
    }

    // TODO: rework API
    animator->Animations.resize(1);
    animator->Animations[0] = animation;
    animator->Play(0);
}

void ComponentsInternalCalls::Animator_Play(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto animator = entity->GetComponent<Animator>();
    if (animator == nullptr)
    {
        Log::LogError("No animator component attached to entity " + std::to_string(entityID));
        return;
    }

    animator->Play();
}

void ComponentsInternalCalls::Animator_Pause(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto animator = entity->GetComponent<Animator>();
    if (animator == nullptr)
    {
        Log::LogError("No animator component attached to entity " + std::to_string(entityID));
        return;
    }

    animator->Pause();
}

void ComponentsInternalCalls::Animator_Stop(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto animator = entity->GetComponent<Animator>();
    if (animator == nullptr)
    {
        Log::LogError("No animator component attached to entity " + std::to_string(entityID));
        return;
    }

    animator->Stop();
}

void ComponentsInternalCalls::Animator_Restart(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto animator = entity->GetComponent<Animator>();
    if (animator == nullptr)
    {
        Log::LogError("No animator component attached to entity " + std::to_string(entityID));
        return;
    }

    animator->Restart();
}

int64_t ComponentsInternalCalls::Camera_GetMainEntityWithMainCamera()
{
    return Application::Instance->GetCurrentScene()->GetMainCamera()->ParentEntity->ID;
}

float ComponentsInternalCalls::Camera_GetWidth(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return camera->GetWidth();
}

void ComponentsInternalCalls::Camera_SetWidth(int64_t entityID, float width)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    camera->SetWidth(width);
}

float ComponentsInternalCalls::Camera_GetHeight(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return camera->GetHeight();
}

void ComponentsInternalCalls::Camera_SetHeight(int64_t entityID, float height)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    camera->SetHeight(height);
}

float ComponentsInternalCalls::Camera_GetNearClippingPlane(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return camera->GetNearClippingPlane();
}

void ComponentsInternalCalls::Camera_SetNearClippingPlane(int64_t entityID, float distance)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    return camera->SetNearClippingPlane(distance);
}

float ComponentsInternalCalls::Camera_GetFarClippingPlane(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return camera->GetFarClippingPlane();
}

void ComponentsInternalCalls::Camera_SetFarClippingPlane(int64_t entityID, float distance)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    camera->SetFarClippingPlane(distance);
}

int ComponentsInternalCalls::Camera_GetResizingMode(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return camera->GetResizeMode();
}

void ComponentsInternalCalls::Camera_SetResizingMode(int64_t entityID, int mode)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    camera->SetResizeMode((CameraResizeModes::CameraResizeMode)mode);
}

void ComponentsInternalCalls::Camera_WorldToScreenPoint(int64_t entityID, glm::vec2* worldPoint, glm::vec2* screenPoint)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    glm::vec2 result = camera->WorldToScreenPoint(*worldPoint);
    screenPoint->x = result.x;
    screenPoint->y = result.y;
}

void ComponentsInternalCalls::Camera_ScreenToWorldPoint(int64_t entityID, glm::vec2* screenPoint, glm::vec2* worldPoint)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto camera = entity->GetComponent<Camera>();
    if (camera == nullptr)
    {
        Log::LogError("No camera component attached to entity " + std::to_string(entityID));
        return;
    }

    glm::vec2 result = camera->ScreenToWorldPoint(*screenPoint);
    worldPoint->x = result.x;
    worldPoint->y = result.y;
}

float ComponentsInternalCalls::RigidBody_GetMass(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto rb = entity->GetComponent<RigidBody>();
    if (rb == nullptr)
    {
        Log::LogError("No rigid body component attached to entity " + std::to_string(entityID));
        return 0;
    }

    return rb->GetMass();
}

void ComponentsInternalCalls::RigidBody_SetMass(int64_t entityID, float mass)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rb = entity->GetComponent<RigidBody>();
    if (rb == nullptr)
    {
        Log::LogError("No rigid body component attached to entity " + std::to_string(entityID));
        return;
    }

    rb->SetMass(mass);
}

RigidBodyTypes::RigidBodyType ComponentsInternalCalls::RigidBody_GetRigidBodyType(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return RigidBodyTypes::None;
    }

    auto rb = entity->GetComponent<RigidBody>();
    if (rb == nullptr)
    {
        Log::LogError("No rigid body component attached to entity " + std::to_string(entityID));
        return RigidBodyTypes::None;
    }

    rb->GetType();
}

void ComponentsInternalCalls::RigidBody_SetRigidBodyType(int64_t entityID, RigidBodyTypes::RigidBodyType type)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rb = entity->GetComponent<RigidBody>();
    if (rb == nullptr)
    {
        Log::LogError("No rigid body component attached to entity " + std::to_string(entityID));
        return;
    }

    rb->SetType(type);
}

void ComponentsInternalCalls::BoxCollider_GetSize(int64_t entityID, glm::vec2* size)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto bc = entity->GetComponent<BoxCollider>();
    if (bc == nullptr)
    {
        Log::LogError("No box collider component attached to entity " + std::to_string(entityID));
        return;
    }

    auto s = bc->GetSize();
    size->x = s.x;
    size->y = s.y;
}

void ComponentsInternalCalls::BoxCollider_SetSize(int64_t entityID, glm::vec2* size)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto bc = entity->GetComponent<BoxCollider>();
    if (bc == nullptr)
    {
        Log::LogError("No box collider component attached to entity " + std::to_string(entityID));
        return;
    }

    bc->SetSize(*size);
}

