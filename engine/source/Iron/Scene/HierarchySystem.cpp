#include "HierarchySystem.h"
#include "Hierarchy.h"

void HierarchySystem::OnComponentAdded(EntityID entityID, HierarchyNode& component)
{

}

void HierarchySystem::OnComponentRemoved(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    // Remove child from its parent and keep all links valid
    if (component.ParentNode != NULL_ENTITY)
    {
        RemoveChildFromItsParent(Registry, component);
    }
    // Delete all children entities
    DeleteRecursively(Registry, component);

    _lock = false;
}

void HierarchySystem::OnEntityEnabled(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    SetActiveRecursively(Registry, component, true);

    _lock = false;
}

void HierarchySystem::OnEntityDisabled(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    SetActiveRecursively(Registry, component, false);

    _lock = false;
}
