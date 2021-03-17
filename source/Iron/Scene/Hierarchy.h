#pragma once

#include "../EntityComponentSystem/Entity.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "HierarchyNode.h"

template<typename Function>
void ForeachChildren(EntitiesRegistry* registry, HierarchyNode& parentNode, Function function)
{
    if (parentNode.ChildrenCount == 0)
        return;

    EntityID currentNodeID = parentNode.FirstChildNode;
    for (uint32_t i = 0; i < parentNode.ChildrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        function(registry, currentNodeID);
        // Recursively call for children
        ForeachChildren(registry, currentChildNode, function);

        currentNodeID = currentChildNode.NextNode;
    }
}

void UpdateChildrenDepthAndSetDirty(EntitiesRegistry* registry, HierarchyNode& parentNode);

void LinkChildToParent(EntitiesRegistry* registry, EntityID child, EntityID parent);

bool CheckIsParentUpwards(EntitiesRegistry* registry, EntityID child, EntityID parent);

std::vector<EntityID> GetAllChildren(EntitiesRegistry* registry, EntityID parent);
