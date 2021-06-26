#include "Hierarchy.h"
#include "Transformation.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../UI/RectTransformation.h"

void UpdateChildrenDepthAndSetDirty(EntitiesRegistry* registry, HierarchyNode& parentNode)
{
    if (parentNode.ChildrenCount == 0)
        return;

    auto& currentChildNode = registry->GetComponent<HierarchyNode>(parentNode.FirstChildNode);
    for (uint32_t i = 0; i < parentNode.ChildrenCount; ++i)
    {
        // This is valid because children nodes links are looped
        currentChildNode = registry->GetComponent<HierarchyNode>(currentChildNode.NextNode);
        currentChildNode.HierarchyDepth = parentNode.HierarchyDepth + 1;
        currentChildNode.IsDirty = true;
        // Recursively update children
        UpdateChildrenDepthAndSetDirty(registry, currentChildNode);
    }
}

void LinkChildToParent(EntitiesRegistry* registry, EntityID child, EntityID parent)
{
    if (child == NULL_ENTITY)
        return;

    auto& childNode = registry->GetComponent<HierarchyNode>(child);

    if (child == parent || parent != NULL_ENTITY && childNode.ParentNode == parent)
        return;

    // Check if child is parent for new parent (possible loop)
    if (CheckIsParentUpwards(registry, parent, child))
    {
        Log::LogWarning("Trying to create hierarchy loop");
        return;
    }

    // Cache child global transformations (not for UI)
    glm::vec3 positionCache, rotationCache, scaleCache;
    bool isTransformation = registry->HasComponent<Transformation>(child);
    if (isTransformation)
    {
        auto& childTransformation = registry->GetComponent<Transformation>(child);
        positionCache = childTransformation.GetPosition();
        rotationCache = childTransformation.GetRotation();
        scaleCache = childTransformation.GetScale();
    }
    else if (registry->HasComponent<RectTransformation>(child))
    {
        // If any UI element changed hierarchy - rebuild all sorting orders
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();
    }

    HierarchyParent& prevParent = childNode.ParentNode == NULL_ENTITY
                                       ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
                                       : registry->GetComponent<HierarchyNode>(childNode.ParentNode);
    // Child already has a parent, we need to change links
    RemoveChildFromItsParent(registry, childNode, prevParent);
    if (childNode.ParentNode != NULL_ENTITY)
        UpdateThicknessUpwards(registry, childNode.ParentNode, -(int)childNode.Thickness);

    HierarchyParent& hierarchyParent = parent == NULL_ENTITY
                                       ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
                                       : registry->GetComponent<HierarchyNode>(parent);
    if (parent == NULL_ENTITY)
    {
        // Removing parent from this child, now it will be on top of hierarchy
        childNode.ParentNode = NULL_ENTITY;
        childNode.HierarchyDepth = 0;
        childNode.NextNode = NULL_ENTITY;
        childNode.PreviousNode = NULL_ENTITY;
    }
    else
    {
        // Moving this child to another parent
        auto& parentNode = registry->GetComponent<HierarchyNode>(parent);
        childNode.ParentNode = parent;
        childNode.HierarchyDepth = parentNode.HierarchyDepth + 1;
        UpdateThicknessUpwards(registry, parentNode.Owner, (int)childNode.Thickness);
    }

    if (hierarchyParent.ChildrenCount == 0)
    {
        hierarchyParent.FirstChildNode = child;
        hierarchyParent.ChildrenCount = 1;
        // Link first child to itself to ease further children list changes
        childNode.NextNode = child;
        childNode.PreviousNode = child;
    }
    else
    {
        hierarchyParent.ChildrenCount++;
        // Insert this node between first child and previously last (effectively in the end)
        auto& firstChildNode = registry->GetComponent<HierarchyNode>(hierarchyParent.FirstChildNode);
        auto& lastChildNode = registry->GetComponent<HierarchyNode>(firstChildNode.PreviousNode);
        // We are now next for previously last child
        lastChildNode.NextNode = child;
        childNode.PreviousNode = firstChildNode.PreviousNode;
        // And previous for first child
        firstChildNode.PreviousNode = child;
        childNode.NextNode = hierarchyParent.FirstChildNode;
    }
    childNode.NodeIndex = hierarchyParent.ChildrenCount;

    // We need to recalculate HierarchyDepth for all children
    childNode.IsDirty = true;
    UpdateChildrenDepthAndSetDirty(registry, childNode);

    // Apply saved global position, so child will not move after changing parent (not for UI)
    if (isTransformation)
    {
        auto& childTransformation = registry->GetComponent<Transformation>(child);
        childTransformation.SetPosition(positionCache);
        childTransformation.SetRotation(rotationCache);
        childTransformation.SetScale(scaleCache);
    }

    // Apply new parent's active state to child entity (null parent similar to active)
    bool isParentActiveGlobally = parent == NULL_ENTITY || (registry->EntityGetState(parent) & EntityStates::IsActive);
    registry->EntitySetActive(child, isParentActiveGlobally, false);
}

void RemoveChildFromItsParent(EntitiesRegistry* registry, HierarchyNode& childNode, HierarchyParent& hierarchyParent)
{
    if (childNode.NodeIndex == -1)
        return;

    // Remove child from its parent and keep all links valid
    if (hierarchyParent.ChildrenCount == 1)
    {
        // This child was the only child of its parent, remove all children then
        hierarchyParent.ChildrenCount = 0;
        hierarchyParent.FirstChildNode = NULL_ENTITY;
    }
    else
    {
        // This child was part of parent children, change links to keep hierarchy
        hierarchyParent.ChildrenCount--;
        if (hierarchyParent.FirstChildNode == childNode.Owner)
            hierarchyParent.FirstChildNode = childNode.NextNode;
        auto& prevPrevNode = registry->GetComponent<HierarchyNode>(childNode.PreviousNode);
        auto& prevNextNode = registry->GetComponent<HierarchyNode>(childNode.NextNode);
        prevPrevNode.NextNode = childNode.NextNode;
        prevNextNode.PreviousNode = childNode.PreviousNode;

        // Recalculating all previous parent's children indices
        // TODO: performance bottleneck when many children get mass destroyed (maybe add dirty flag)
        EntityID currentNodeID = hierarchyParent.FirstChildNode;
        for (uint32_t i = 0; i < hierarchyParent.ChildrenCount; ++i)
        {
            auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);
            currentNode.NodeIndex = i;
            currentNodeID = currentNode.NextNode;
        }
    }

    if (registry->HasComponent<RectTransformation>(childNode.Owner))
    {
        // If any UI element changed hierarchy - rebuild all sorting orders
        Application::Instance->GetCurrentScene()->GetUILayer()->SetSortingOrderDirty();
    }
}

bool CheckIsParentUpwards(EntitiesRegistry* registry, EntityID child, EntityID parent)
{
    if (parent == NULL_ENTITY)
        return true;

    EntityID currentNodeID = child;
    while (currentNodeID != NULL_ENTITY)
    {
        auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        if (currentNode.ParentNode == parent)
            return true;

        currentNodeID = currentNode.ParentNode;
    }

    return false;
}

void UpdateThicknessUpwards(EntitiesRegistry* registry, EntityID nodeID, int diff)
{
    EntityID currentNodeID = nodeID;
    while (currentNodeID != NULL_ENTITY)
    {
        auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        currentNode.Thickness += diff;

        currentNodeID = currentNode.ParentNode;
    }
}

std::vector<EntityID> GetAllChildren(EntitiesRegistry* registry, EntityID parent)
{
    if (parent == NULL_ENTITY)
    {
        Log::LogError("Can't get children for NULL entity");
        return std::vector<EntityID>();
    }

    auto& parentNode = registry->GetComponent<HierarchyNode>(parent);
    std::vector<EntityID> result;
    if (parentNode.ChildrenCount == 0)
        return result;

    result.reserve(parentNode.ChildrenCount);
    EntityID currentNodeID = parentNode.FirstChildNode;
    for (uint32_t i = 0; i < parentNode.ChildrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        result.push_back(currentChildNode.PreviousNode);

        currentNodeID = currentChildNode.NextNode;
    }

    return result;
}

void SetActiveRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode, bool active)
{
    if (parentNode.ChildrenCount == 0)
        return;

    EntityID currentNodeID = parentNode.FirstChildNode;
    uint32_t childrenCount = parentNode.ChildrenCount;
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        // Save next node, because currentChildNode reference will move after EntitySetActive
        auto nextNodeBackup = currentChildNode.NextNode;
        // If component is already in deactivated state itself, then there is no need to apply any other state
        if (registry->EntityGetState(currentNodeID) & EntityStates::IsActiveSelf)
        {
            // Recursively call for children
            SetActiveRecursively(registry, currentChildNode, active);
            registry->EntitySetActive(currentNodeID, active, false);
        }

        currentNodeID = nextNodeBackup;
    }
}

void DeleteRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode)
{
    if (parentNode.ChildrenCount == 0)
        return;

    EntityID currentNodeID = parentNode.FirstChildNode;
    uint32_t childrenCount = parentNode.ChildrenCount;
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        // Save next node, because currentChildNode reference will move after DeleteEntity
        auto nextNodeBackup = currentChildNode.NextNode;

        // Recursively call for children
        DeleteRecursively(registry, currentChildNode);
        registry->DeleteEntity(currentNodeID);

        currentNodeID = nextNodeBackup;
    }
}
