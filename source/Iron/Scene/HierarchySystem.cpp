#include "HierarchySystem.h"
#include "Hierarchy.h"
#include "../Core/Application.h"

void HierarchySystem::OnComponentAdded(EntityID entityID, HierarchyNode& component)
{

}

void HierarchySystem::OnComponentRemoved(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    // Remove child from its parent and keep all links valid
    if (component.ParentNode != NULL_ENTITY)
    {
        RemoveChildFromItsParent(entitiesRegistry, component);
    }
    // Delete all children entities
    DeleteRecursively(entitiesRegistry, component);

    _lock = false;
}

void HierarchySystem::OnEntityEnabled(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    SetActiveRecursively(entitiesRegistry, component, true);

    _lock = false;
}

void HierarchySystem::OnEntityDisabled(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    SetActiveRecursively(entitiesRegistry, component, false);

    _lock = false;
}
