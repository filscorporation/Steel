#pragma once

#include "../EntityComponentSystem/Entity.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "HierarchyNode.h"

template<typename Function>
void ForeachChildren(EntitiesRegistry* registry, HierarchyNode& parentNode, Function function)
{
    if (parentNode.ChildrenCount == 0)
        return;

    auto& currentChildNode = registry->GetComponent<HierarchyNode>(parentNode.FirstChildNode);
    for (uint32_t i = 0; i < parentNode.ChildrenCount; ++i)
    {
        // This is valid because children nodes links are looped
        currentChildNode = registry->GetComponent<HierarchyNode>(currentChildNode.NextNode);
        function(registry, currentChildNode.Owner);
        // Recursively call for children
        ForeachChildren(registry, currentChildNode, function);
    }
}

void UpdateChildrenDepthAndSetDirty(EntitiesRegistry* registry, HierarchyNode& parentNode);

void LinkChildToParent(EntitiesRegistry* registry, EntityID child, EntityID parent);

std::vector<EntityID> GetAllChildren(EntitiesRegistry* registry, EntityID parent);
