#pragma once

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "HierarchyNode.h"

template<typename Function>
void ForeachChildren(EntitiesRegistry* registry, HierarchyNode& parentNode, Function function)
{
    if (parentNode.GetChildrenCount() == 0)
        return;

    EntityID currentNodeID = parentNode.GetFirstChildNode();
    for (uint32_t i = 0; i < parentNode.GetChildrenCount(); ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        function(registry, currentNodeID);
        // Recursively call for children
        ForeachChildren(registry, currentChildNode, function);

        currentNodeID = currentChildNode.GetNextNode();
    }
}

void UpdateChildrenDepthAndSetDirty(EntitiesRegistry* registry, HierarchyNode& parentNode);

void LinkChildToParent(EntitiesRegistry* registry, EntityID child, EntityID parent);

void UpdateHierarchyDependantProperties(EntitiesRegistry* registry, HierarchyNode& targetNode);

void RemoveChildFromItsParent(EntitiesRegistry* registry, HierarchyNode& childNode, HierarchyParent& hierarchyParent);

bool CheckIsParentUpwards(EntitiesRegistry* registry, EntityID child, EntityID parent);

short GetClippingLevelUpwards(EntitiesRegistry* registry, EntityID child);

void UpdateThicknessUpwards(EntitiesRegistry* registry, EntityID nodeID, int diff);

std::vector<EntityID> GetAllChildren(EntitiesRegistry* registry, EntityID parent);

void SetActiveRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode, bool active);

void DeleteRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode);
