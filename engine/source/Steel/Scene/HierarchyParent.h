#pragma once

#include "../EntityComponentSystem/Entity.h"

#include <cstdint>

class HierarchyParent
{
public:
    virtual ~HierarchyParent() = default;

    uint32_t GetChildrenCount() const;
    void SetChildrenCount(uint32_t childrenCount);
    EntityID GetFirstChildNode() const;
    void SetFirstChildNode(EntityID firstChildNode);

protected:
    uint32_t _childrenCount = 0;
    EntityID _firstChildNode = NULL_ENTITY;
};
