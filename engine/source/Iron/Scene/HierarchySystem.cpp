#include "HierarchySystem.h"
#include "Hierarchy.h"
#include "../Core/Application.h"
#include "../UI/RectTransformation.h"

void HierarchySystem::OnComponentAdded(EntityID entityID, HierarchyNode& component)
{

}

void HierarchySystem::OnComponentRemoved(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    // Remove child from its parent and keep all links valid
    HierarchyParent& hierarchyParent =
            component.ParentNode == NULL_ENTITY
               ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
               : ComponentSystem<HierarchyNode>::Registry->GetComponent<HierarchyNode>(component.ParentNode);
    RemoveChildFromItsParent(Registry, component, hierarchyParent);
    if (component.ParentNode != NULL_ENTITY)
        UpdateThicknessUpwards(ComponentSystem<HierarchyNode>::Registry, component.ParentNode, -(int)component.Thickness);
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
    if (ComponentSystem<HierarchyNode>::Registry->HasComponent<RectTransformation>(entityID))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    _lock = false;
}

void HierarchySystem::OnEntityDisabled(EntityID entityID, HierarchyNode& component)
{
    if (_lock)
        return;
    _lock = true;

    SetActiveRecursively(Registry, component, false);
    if (ComponentSystem<HierarchyNode>::Registry->HasComponent<RectTransformation>(entityID))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    _lock = false;
}
