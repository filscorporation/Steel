#include "HierarchyNode.h"
#include "Hierarchy.h"
#include "Steel/Core/Application.h"
#include "Steel/UI/RectTransformation.h"

bool lock = false;

void HierarchyNode::RegisterType()
{
    REGISTER_TYPE(HierarchyNode);
    REGISTER_ATTRIBUTE(HierarchyNode, "childrenCount", GetChildrenCount, SetChildrenCount, uint32_t, AttributeFlags::Public);
    REGISTER_ID_ATTRIBUTE(HierarchyNode, "firstChildNode", GetFirstChildNode, SetFirstChildNode, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(HierarchyNode, "hierarchyDepth", GetHierarchyDepth, SetHierarchyDepth, uint32_t, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(HierarchyNode, "nodeIndex", GetNodeIndex, SetNodeIndex, uint32_t, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(HierarchyNode, "thickness", GetThickness, SetThickness, uint32_t, AttributeFlags::Public);
    REGISTER_ID_ATTRIBUTE(HierarchyNode, "previousNode", GetPreviousNode, SetPreviousNode, AttributeFlags::Public);
    REGISTER_ID_ATTRIBUTE(HierarchyNode, "nextNode", GetNextNode, SetNextNode, AttributeFlags::Public);
    REGISTER_ID_ATTRIBUTE(HierarchyNode, "parentNode", GetParentNode, SetParentNode, AttributeFlags::Public);
}

void HierarchyNode::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (lock || entitiesRegistry->IsCleared())
        return;
    lock = true;

    // Remove child from its parent and keep all links valid
    HierarchyParent& hierarchyParent =
            _parentNode == NULL_ENTITY
            ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
            : entitiesRegistry->GetComponent<HierarchyNode>(_parentNode);
    RemoveChildFromItsParent(entitiesRegistry, (*this), hierarchyParent);
    if (_parentNode != NULL_ENTITY)
    {
        UpdateThicknessUpwards(entitiesRegistry, _parentNode, -(int)_thickness);
        entitiesRegistry->GetComponent<HierarchyNode>(_parentNode).IsDirty = true;
    }
    // Delete all children entities
    DeleteRecursively(entitiesRegistry, (*this));

    lock = false;
}

void HierarchyNode::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (lock)
        return;
    lock = true;

    EntityID ownerBackup = Owner;
    IsDirty = true;
    SetActiveRecursively(entitiesRegistry, (*this), true);
    if (entitiesRegistry->HasComponent<RectTransformation>(ownerBackup))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    lock = false;
}

void HierarchyNode::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    if (lock)
        return;
    lock = true;

    EntityID ownerBackup = Owner;
    IsDirty = true;
    SetActiveRecursively(entitiesRegistry, (*this), false);
    if (entitiesRegistry->HasComponent<RectTransformation>(ownerBackup))
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();

    lock = false;
}

uint32_t HierarchyNode::GetHierarchyDepth() const
{
    return _hierarchyDepth;
}

void HierarchyNode::SetHierarchyDepth(uint32_t hierarchyDepth)
{
    _hierarchyDepth = hierarchyDepth;
}

uint32_t HierarchyNode::GetNodeIndex() const
{
    return _nodeIndex;
}

void HierarchyNode::SetNodeIndex(uint32_t nodeIndex)
{
    _nodeIndex = nodeIndex;
}

uint32_t HierarchyNode::GetThickness() const
{
    return _thickness;
}

void HierarchyNode::SetThickness(uint32_t thickness)
{
    _thickness = thickness;
}

EntityID HierarchyNode::GetPreviousNode() const
{
    return _previousNode;
}

void HierarchyNode::SetPreviousNode(EntityID previousNode)
{
    _previousNode = previousNode;
}

EntityID HierarchyNode::GetNextNode() const
{
    return _nextNode;
}

void HierarchyNode::SetNextNode(EntityID nextNode)
{
    _nextNode = nextNode;
}

EntityID HierarchyNode::GetParentNode() const
{
    return _parentNode;
}

void HierarchyNode::SetParentNode(EntityID parentNode)
{
    _parentNode = parentNode;
}
