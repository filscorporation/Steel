#include "HierarchyNode.h"
#include "Hierarchy.h"
#include "../Core/Application.h"
#include "../UI/RectTransformation.h"

bool lock = false;

void HierarchyNode::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (lock || entitiesRegistry->IsCleared())
        return;
    lock = true;

    // Remove child from its parent and keep all links valid
    HierarchyParent& hierarchyParent =
            ParentNode == NULL_ENTITY
            ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
            : entitiesRegistry->GetComponent<HierarchyNode>(ParentNode);
    RemoveChildFromItsParent(entitiesRegistry, (*this), hierarchyParent);
    if (ParentNode != NULL_ENTITY)
    {
        UpdateThicknessUpwards(entitiesRegistry, ParentNode, -(int)Thickness);
        entitiesRegistry->GetComponent<HierarchyNode>(ParentNode).IsDirty = true;
    }
    // Delete all children entities
    DeleteRecursively(entitiesRegistry, (*this));

    lock = false;
}

void HierarchyNode::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (lock)
        return;
    lock = true;

    EntityID ownerBackup = Owner;
    IsDirty = true;
    SetActiveRecursively(entitiesRegistry, (*this), true);
    if (entitiesRegistry->HasComponent<RectTransformation>(ownerBackup))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    lock = false;
}

void HierarchyNode::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    if (lock)
        return;
    lock = true;

    EntityID ownerBackup = Owner;
    IsDirty = true;
    SetActiveRecursively(entitiesRegistry, (*this), false);
    if (entitiesRegistry->HasComponent<RectTransformation>(ownerBackup))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    lock = false;
}
