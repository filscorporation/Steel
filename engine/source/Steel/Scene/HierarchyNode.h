#pragma once

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/Entity.h"
#include "HierarchyParent.h"

#define DEFAULT_THICKNESS 1

class HierarchyNode : public Component, public HierarchyParent
{
public:
    explicit HierarchyNode(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    uint32_t HierarchyDepth = 0;
    uint32_t NodeIndex = -1; // Value of -1 will show that node just initialized
    uint32_t Thickness = DEFAULT_THICKNESS;
    EntityID PreviousNode = NULL_ENTITY;
    EntityID NextNode = NULL_ENTITY;
    EntityID ParentNode = NULL_ENTITY;
    bool IsDirty = false;
};
