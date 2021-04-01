#include "UISystem.h"
#include "../Core/Application.h"
#include "UIEventHandler.h"

void UISystem::OnComponentAdded(EntityID entityID, UIText& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    CheckRectTransformation(entitiesRegistry, entityID);

    component._font = Application::Instance->GetResourcesManager()->DefaultFont();
}

void UISystem::OnComponentRemoved(EntityID entityID, UIText& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    component.ForeachLetterDelete(entitiesRegistry, component.lettersCount);
    component.lastLetterID = NULL_ENTITY;
    component.lettersCount = 0;
}

void UISystem::OnEntityEnabled(EntityID entityID, UIText& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    component.ForeachLetterSetActive(entitiesRegistry, true);
}

void UISystem::OnEntityDisabled(EntityID entityID, UIText& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    component.ForeachLetterSetActive(entitiesRegistry, false);
}

void UISystem::OnComponentAdded(EntityID entityID, UIImage& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    CheckRectTransformation(entitiesRegistry, entityID);
    TryAddUIRenderer(entitiesRegistry, entityID);
    TryAddEventHandler(entitiesRegistry, entityID);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIImage& component)
{ }

void UISystem::OnEntityEnabled(EntityID entityID, UIImage& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UIImage& component)
{ }

void UISystem::OnComponentAdded(EntityID entityID, UIButton& component)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    CheckRectTransformation(entitiesRegistry, entityID);
    TryAddUIRenderer(entitiesRegistry, entityID);
    TryAddEventHandler(entitiesRegistry, entityID);

    auto& eh = entitiesRegistry->GetComponent<UIEventHandler>(entityID);

    eh.EventCallback = UIButton::HandleEvent;
    eh.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                    | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    component._transitionsInfo.TransitionType = ButtonTransitionTypes::ColorShift;
    component._transitionsInfo.TransitionDuration = 0.3f;
    component._transitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    component._transitionsInfo.Hovered.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    component._transitionsInfo.Clicked.FromColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    component._transitionsInfo.Disabled.FromColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
}

void UISystem::OnComponentRemoved(EntityID entityID, UIButton& component)
{
    if (component.isInTransition)
    {
        component.isInTransition = false;
        Application::Instance->GetCurrentScene()->GetUILayer()->RemoveButtonFromUpdateQueue(entityID);
    }
}

void UISystem::OnEntityEnabled(EntityID entityID, UIButton& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UIButton& component)
{ }

void UISystem::CheckRectTransformation(EntitiesRegistry* entitiesRegistry, EntityID entityID)
{
    if (!entitiesRegistry->HasComponent<RectTransformation>(entityID))
    {
        Log::LogError("Adding UIComponents to objects without RectTransformation will lead to undefined behaviour. "
                      "Create UI objects using UILayout, not Scene");
    }
}

void UISystem::TryAddUIRenderer(EntitiesRegistry* entitiesRegistry, EntityID entityID)
{
    if (entitiesRegistry->HasComponent<UIRenderer>(entityID))
    {
        Log::LogError("Entity " + std::to_string(entityID) + " already has UIRenderer component attached, "
                                                                    "this may lead to conflicts. Keep one UIComponent per object");
    }
    else
    {
        entitiesRegistry->AddComponent<UIRenderer>(entityID);
    }
}

void UISystem::TryAddEventHandler(EntitiesRegistry* entitiesRegistry, EntityID entityID)
{
    if (entitiesRegistry->HasComponent<UIEventHandler>(entityID))
    {
        Log::LogError("Entity " + std::to_string(entityID) + " already has UIEventHandler component attached, "
                                                                    "this may lead to conflicts. Keep one UIEventHandler per object");
    }
    else
    {
        entitiesRegistry->AddComponent<UIEventHandler>(entityID);
    }
}
