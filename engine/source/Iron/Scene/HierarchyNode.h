#pragma once

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/Entity.h"

class HierarchyNode : public Component
{
public:
    explicit HierarchyNode(EntityID ownerEntityID) : Component(ownerEntityID) { }

    uint32_t ChildrenCount = 0;
    uint32_t HierarchyDepth = 0;
    EntityID FirstChildNode = NULL_ENTITY;
    EntityID PreviousNode = NULL_ENTITY;
    EntityID NextNode = NULL_ENTITY;
    EntityID ParentNode = NULL_ENTITY;
    bool IsDirty = false;
};
