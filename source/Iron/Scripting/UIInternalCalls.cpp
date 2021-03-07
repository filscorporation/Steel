#include "UIInternalCalls.h"
#include "../UI/UIImage.h"
#include "../Core/Log.h"
#include "../Core/Application.h"

void UIInternalCalls::RectTransformation_GetAnchorMin(int64_t entityID, glm::vec2* anchor)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    anchor->x = rt->GetAnchorMin().x;
    anchor->y = rt->GetAnchorMin().y;
}

void UIInternalCalls::RectTransformation_SetAnchorMin(int64_t entityID, glm::vec2* anchor)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetAnchorMin(*anchor);
}

void UIInternalCalls::RectTransformation_GetAnchorMax(int64_t entityID, glm::vec2* anchor)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    anchor->x = rt->GetAnchorMax().x;
    anchor->y = rt->GetAnchorMax().y;
}

void UIInternalCalls::RectTransformation_SetAnchorMax(int64_t entityID, glm::vec2* anchor)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetAnchorMax(*anchor);
}

glm::vec3 UIInternalCalls::RectTransformation_GetAnchoredPosition(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return glm::vec3(0.0f);
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return glm::vec3(0.0f);
    }

    return rt->GetAnchoredPosition();
}

void UIInternalCalls::RectTransformation_SetAnchoredPosition(int64_t entityID, glm::vec3 position)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetAnchoredPosition(position);
}

void UIInternalCalls::RectTransformation_GetOffsetMin(int64_t entityID, glm::vec2* offset)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    offset->x = rt->GetOffsetMin().x;
    offset->y = rt->GetOffsetMin().y;
}

void UIInternalCalls::RectTransformation_SetOffsetMin(int64_t entityID, glm::vec2* offset)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetOffsetMin(*offset);
}

void UIInternalCalls::RectTransformation_GetOffsetMax(int64_t entityID, glm::vec2* offset)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    offset->x = rt->GetOffsetMax().x;
    offset->y = rt->GetOffsetMax().y;
}

void UIInternalCalls::RectTransformation_SetOffsetMax(int64_t entityID, glm::vec2* offset)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetOffsetMax(*offset);
}

void UIInternalCalls::RectTransformation_GetPivot(int64_t entityID, glm::vec2* pivot)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    pivot->x = rt->GetPivot().x;
    pivot->y = rt->GetPivot().y;
}

void UIInternalCalls::RectTransformation_SetPivot(int64_t entityID, glm::vec2* pivot)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetPivot(*pivot);
}

void UIInternalCalls::RectTransformation_GetSize(int64_t entityID, glm::vec2* size)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    size->x = rt->GetSize().x;
    size->y = rt->GetSize().y;
}

void UIInternalCalls::RectTransformation_SetSize(int64_t entityID, glm::vec2* size)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto rt = entity->GetComponent<RectTransformation>();
    if (rt == nullptr)
    {
        Log::LogError("No rect transformation component attached to entity " + std::to_string(entityID));
        return;
    }

    rt->SetSize(*size);
}

uint64_t UIInternalCalls::UIImage_GetSprite(int64_t entityID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return 0;
    }

    auto ui = entity->GetComponent<UIImage>();
    if (ui == nullptr)
    {
        Log::LogError("No UI image component attached to entity " + std::to_string(entityID));
        return 0;
    }

    auto image = ui->GetImage();

    return image == nullptr ? 0 : image->ID;
}

void UIInternalCalls::UIImage_SetSprite(int64_t entityID, int64_t spriteID)
{
    auto entity = Application::Instance->GetCurrentScene()->GetEntity(entityID);
    if (entity == nullptr)
    {
        Log::LogError("Entity does not exist: " + std::to_string(entityID));
        return;
    }

    auto ui = entity->GetComponent<UIImage>();
    if (ui == nullptr)
    {
        Log::LogError("No UI image component attached to entity " + std::to_string(entityID));
        return;
    }

    ui->SetImage(Application::Instance->GetResourcesManager()->GetImage(spriteID));
}