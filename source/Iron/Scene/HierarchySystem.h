#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "HierarchyNode.h"

class HierarchySystem : public ComponentSystem<HierarchyNode>
{
public:
    void OnComponentAdded(EntityID entityID, HierarchyNode& component) override;
    void OnComponentRemoved(EntityID entityID, HierarchyNode& component) override;
    void OnEntityEnabled(EntityID entityID, HierarchyNode& component) override;
    void OnEntityDisabled(EntityID entityID, HierarchyNode& component) override;

private:
    bool _lock = false;
};
