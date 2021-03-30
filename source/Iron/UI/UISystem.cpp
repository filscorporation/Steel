#include "UISystem.h"
#include "../Core/Application.h"

void UISystem::OnComponentAdded(EntityID entityID, UIText& component)
{

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
