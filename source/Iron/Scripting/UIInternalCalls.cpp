#include "UIInternalCalls.h"
#include "../UI/UIImage.h"
#include "../Core/Log.h"
#include "../Core/Application.h"

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