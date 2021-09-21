#pragma once

class HierarchyParent
{
public:
    virtual ~HierarchyParent() = default;

    uint32_t ChildrenCount = 0;
    EntityID FirstChildNode = NULL_ENTITY;
};
