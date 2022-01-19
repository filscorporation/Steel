#include "Hierarchy.h"
#include "Transformation.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/UI/RectTransformation.h"
#include "Steel/UI/UIElements/UIClipping.h"
#include "Steel/UI/UIElements/UIImage.h"
#include "Steel/UI/UIElements/UIText.h"

void UpdateChildrenDepthAndSetDirty(EntitiesRegistry* registry, HierarchyNode& parentNode)
{
    if (parentNode.GetChildrenCount() == 0)
        return;

    auto& currentChildNode = registry->GetComponent<HierarchyNode>(parentNode.GetFirstChildNode());
    for (uint32_t i = 0; i < parentNode.GetChildrenCount(); ++i)
    {
        // This is valid because children nodes links are looped
        currentChildNode = registry->GetComponent<HierarchyNode>(currentChildNode.GetNextNode());
        currentChildNode.SetHierarchyDepth(parentNode.GetHierarchyDepth() + 1);
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

    if (child == parent || parent != NULL_ENTITY && childNode.GetParentNode() == parent)
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

    HierarchyParent& prevParent = childNode.GetParentNode() == NULL_ENTITY
                                       ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
                                       : registry->GetComponent<HierarchyNode>(childNode.GetParentNode());
    // Child already has a parent, we need to change links
    RemoveChildFromItsParent(registry, childNode, prevParent);
    if (childNode.GetParentNode() != NULL_ENTITY)
    {
        UpdateThicknessUpwards(registry, childNode.GetParentNode(), -(int)childNode.GetThickness());
        registry->GetComponent<HierarchyNode>(childNode.GetParentNode()).IsDirty = true;
    }

    HierarchyParent& hierarchyParent = parent == NULL_ENTITY
                                       ? (HierarchyParent&)(*Application::Instance->GetCurrentScene())
                                       : registry->GetComponent<HierarchyNode>(parent);
    if (parent == NULL_ENTITY)
    {
        // Removing parent from this child, now it will be on top of hierarchy
        childNode.SetParentNode(NULL_ENTITY);
        childNode.SetHierarchyDepth(0);
        childNode.SetNextNode(NULL_ENTITY);
        childNode.SetPreviousNode(NULL_ENTITY);
    }
    else
    {
        // Moving this child to another parent
        auto& parentNode = registry->GetComponent<HierarchyNode>(parent);
        parentNode.IsDirty = true;
        childNode.SetParentNode(parent);
        childNode.SetHierarchyDepth(parentNode.GetHierarchyDepth() + 1);
        UpdateThicknessUpwards(registry, parentNode.Owner, (int)childNode.GetThickness());
    }

    if (hierarchyParent.GetChildrenCount() == 0)
    {
        hierarchyParent.SetFirstChildNode(child);
        hierarchyParent.SetChildrenCount(1);
        // Link first child to itself to ease further children list changes
        childNode.SetNextNode(child);
        childNode.SetPreviousNode(child);
    }
    else
    {
        hierarchyParent.SetChildrenCount(hierarchyParent.GetChildrenCount() + 1);
        // Insert this node between first child and previously last (effectively in the end)
        auto& firstChildNode = registry->GetComponent<HierarchyNode>(hierarchyParent.GetFirstChildNode());
        auto& lastChildNode = registry->GetComponent<HierarchyNode>(firstChildNode.GetPreviousNode());
        // We are now next for previously last child
        lastChildNode.SetNextNode(child);
        childNode.SetPreviousNode(firstChildNode.GetPreviousNode());
        // And previous for first child
        firstChildNode.SetPreviousNode(child);
        childNode.SetNextNode(hierarchyParent.GetFirstChildNode());
    }
    childNode.SetNodeIndex(hierarchyParent.GetChildrenCount() - 1);

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

    // Recalculate components properties
    UpdateHierarchyDependantProperties(registry, childNode);
}

void UpdateClippingRecursive(EntitiesRegistry* registry, HierarchyNode& currentNode, short clippingLevel)
{
    if (registry->HasComponent<UIClipping>(currentNode.Owner))
    {
        auto& clipping = registry->GetComponent<UIClipping>(currentNode.Owner);
        if (!clipping.WasRemoved())
        {
            clippingLevel++;
            clipping.ClippingLevel = clippingLevel;
        }
    }
    if (registry->HasComponent<UIImage>(currentNode.Owner))
    {
        auto& image = registry->GetComponent<UIImage>(currentNode.Owner);
        image.SetClippingLevel(clippingLevel);
    }
    if (registry->HasComponent<UIText>(currentNode.Owner))
    {
        auto& text = registry->GetComponent<UIText>(currentNode.Owner);
        text.SetClippingLevel(clippingLevel);
    }

    EntityID currentNodeID = currentNode.GetFirstChildNode();
    for (uint32_t i = 0; i < currentNode.GetChildrenCount(); ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        UpdateClippingRecursive(registry, currentChildNode, clippingLevel);

        currentNodeID = currentChildNode.GetNextNode();
    }
}

void UpdateHierarchyDependantProperties(EntitiesRegistry* registry, HierarchyNode& targetNode)
{
    // Update clipping
    short clippingLevel = GetClippingLevelUpwards(registry, targetNode.GetParentNode());
    UpdateClippingRecursive(registry, targetNode, clippingLevel);
}

void RemoveChildFromItsParent(EntitiesRegistry* registry, HierarchyNode& childNode, HierarchyParent& hierarchyParent)
{
    // Node is just initialized
    if (childNode.GetNodeIndex() == -1)
        return;

    // Remove child from its parent and keep all links valid
    if (hierarchyParent.GetChildrenCount() == 1)
    {
        // This child was the only child of its parent, remove all children then
        hierarchyParent.SetChildrenCount(0);
        hierarchyParent.SetFirstChildNode(NULL_ENTITY);
    }
    else
    {
        // This child was part of parent children, change links to keep hierarchy
        hierarchyParent.SetChildrenCount(hierarchyParent.GetChildrenCount() - 1);
        if (hierarchyParent.GetFirstChildNode() == childNode.Owner)
            hierarchyParent.SetFirstChildNode(childNode.GetNextNode());
        auto& prevPrevNode = registry->GetComponent<HierarchyNode>(childNode.GetPreviousNode());
        auto& prevNextNode = registry->GetComponent<HierarchyNode>(childNode.GetNextNode());
        prevPrevNode.SetNextNode(childNode.GetNextNode());
        prevNextNode.SetPreviousNode(childNode.GetPreviousNode());

        // Recalculating all previous parent's children indices
        // TODO: performance bottleneck when many children get mass destroyed (maybe add dirty flag)
        EntityID currentNodeID = hierarchyParent.GetFirstChildNode();
        for (uint32_t i = 0; i < hierarchyParent.GetChildrenCount(); ++i)
        {
            auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);
            currentNode.SetNodeIndex(i);
            currentNodeID = currentNode.GetNextNode();
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
        if (currentNode.GetParentNode() == parent)
            return true;

        currentNodeID = currentNode.GetParentNode();
    }

    return false;
}

short GetClippingLevelUpwards(EntitiesRegistry* registry, EntityID parent)
{
    if (parent == NULL_ENTITY)
        return 0;

    EntityID currentNodeID = parent;
    while (currentNodeID != NULL_ENTITY)
    {
        if (registry->HasComponent<UIClipping>(currentNodeID))
        {
            return registry->GetComponent<UIClipping>(currentNodeID).ClippingLevel;
        }

        auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);

        currentNodeID = currentNode.GetParentNode();
    }

    return 0;
}

void UpdateThicknessUpwards(EntitiesRegistry* registry, EntityID nodeID, int diff)
{
    EntityID currentNodeID = nodeID;
    while (currentNodeID != NULL_ENTITY)
    {
        auto& currentNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        currentNode.SetThickness(currentNode.GetThickness() + diff);

        currentNodeID = currentNode.GetParentNode();
    }
}

std::vector<EntityID> GetAllChildren(EntitiesRegistry* registry, EntityID parent)
{
    if (parent == NULL_ENTITY)
    {
        Log::LogError("Can't get children for NULL entity");
        return {};
    }

    auto& parentNode = registry->GetComponent<HierarchyNode>(parent);
    std::vector<EntityID> result;
    if (parentNode.GetChildrenCount() == 0)
        return result;

    result.reserve(parentNode.GetChildrenCount());
    EntityID currentNodeID = parentNode.GetFirstChildNode();
    for (uint32_t i = 0; i < parentNode.GetChildrenCount(); ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        result.push_back(currentChildNode.GetPreviousNode());

        currentNodeID = currentChildNode.GetNextNode();
    }

    return result;
}

void SetActiveRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode, bool active)
{
    if (parentNode.GetChildrenCount() == 0)
        return;

    EntityID currentNodeID = parentNode.GetFirstChildNode();
    uint32_t childrenCount = parentNode.GetChildrenCount();
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        // Save next node, because currentChildNode reference will move after EntitySetActive
        auto nextNodeBackup = currentChildNode.GetNextNode();
        // If component is already in deactivated state itself, then there is no need to apply any other state
        if (registry->EntityGetState(currentNodeID) & EntityStates::IsActiveSelf)
        {
            // Recursively call for children
            currentChildNode.IsDirty = true;
            SetActiveRecursively(registry, currentChildNode, active);
            registry->EntitySetActive(currentNodeID, active, false);
        }

        currentNodeID = nextNodeBackup;
    }
}

void DeleteRecursively(EntitiesRegistry* registry, HierarchyNode& parentNode)
{
    if (parentNode.GetChildrenCount() == 0)
        return;

    EntityID currentNodeID = parentNode.GetFirstChildNode();
    uint32_t childrenCount = parentNode.GetChildrenCount();
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = registry->GetComponent<HierarchyNode>(currentNodeID);
        // Save next node, because currentChildNode reference will move after DeleteEntity
        auto nextNodeBackup = currentChildNode.GetNextNode();

        // Recursively call for children
        DeleteRecursively(registry, currentChildNode);
        registry->DeleteEntity(currentNodeID);

        currentNodeID = nextNodeBackup;
    }
}
