#pragma once

#include "HierarchyParent.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/Entity.h"

#define DEFAULT_THICKNESS 1

class HierarchyNode : public Component, public HierarchyParent
{
    DEFINE_COMPONENT(HierarchyNode)

public:
    explicit HierarchyNode(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    uint32_t GetHierarchyDepth() const;
    void SetHierarchyDepth(uint32_t hierarchyDepth);
    uint32_t GetNodeIndex() const;
    void SetNodeIndex(uint32_t nodeIndex);
    uint32_t GetThickness() const;
    void SetThickness(uint32_t thickness);
    EntityID GetPreviousNode() const;
    void SetPreviousNode(EntityID previousNode);
    EntityID GetNextNode() const;
    void SetNextNode(EntityID nextNode);
    EntityID GetParentNode() const;
    void SetParentNode(EntityID parentNode);

    bool IsDirty = true;
private:
    uint32_t _hierarchyDepth = 0;
    uint32_t _nodeIndex = -1; // Value of -1 will show that node just initialized
    uint32_t _thickness = DEFAULT_THICKNESS;
    EntityID _previousNode = NULL_ENTITY;
    EntityID _nextNode = NULL_ENTITY;
    EntityID _parentNode = NULL_ENTITY;
};
