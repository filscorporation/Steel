#include "HierarchyParent.h"

uint32_t HierarchyParent::GetChildrenCount() const
{
    return _childrenCount;
}

void HierarchyParent::SetChildrenCount(uint32_t childrenCount)
{
    _childrenCount = childrenCount;
}

EntityID HierarchyParent::GetFirstChildNode() const
{
    return _firstChildNode;
}

void HierarchyParent::SetFirstChildNode(EntityID firstChildNode)
{
    _firstChildNode = firstChildNode;
}
