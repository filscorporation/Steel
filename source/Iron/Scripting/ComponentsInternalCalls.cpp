#include "ComponentsInternalCalls.h"
#include "../Core/Application.h"
#include "../Rendering/SpriteRenderer.h"
#include "../Core/Log.h"
#include "../Animation/Animator.h"

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

    return entity->Transform->GetPosition();
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

    return entity->Transform->GetPosition();
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
