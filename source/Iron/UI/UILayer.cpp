#include "UILayer.h"
#include "UIRenderer.h"
#include "../Scene/Hierarchy.h"
#include "../Scene/NameComponent.h"

EntityID UILayer::CreateUIElement()
{
    return CreateUIElement("New UI element", NULL_ENTITY);
}

EntityID UILayer::CreateUIElement(const char* name, EntityID parent)
{
    auto entity = _scene->CreateEmptyEntity();
    auto& nameComponent = _scene->GetEntitiesRegistry()->AddComponent<NameComponent>(entity);
    nameComponent.Name = name;
    _scene->GetEntitiesRegistry()->AddComponent<RectTransformation>(entity);
    _scene->GetEntitiesRegistry()->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(_scene->GetEntitiesRegistry(), entity, parent);

    return entity;
}

void UILayer::Draw()
{
    // Prepare to draw
    auto entitiesRegistry = _scene->GetEntitiesRegistry();

    // Update rect transformations if needed
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto rtAccessor = entitiesRegistry->GetComponentAccessor<RectTransformation>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (rtAccessor.Has(hierarchyNode.Owner))
            rtAccessor.Get(hierarchyNode.Owner).UpdateTransformation(rtAccessor, hierarchyNode);
    }

    // Sort all UI objects by SortingOrder
    struct
    {
        bool operator()(RectTransformation& a, RectTransformation& b) const
        { return a.GetGlobalSortingOrderCached() < b.GetGlobalSortingOrderCached(); }
    } SOComparer;
    entitiesRegistry->SortComponents<RectTransformation>(SOComparer);
    entitiesRegistry->ApplyOrder<RectTransformation, UIRenderer>();

    // Draw
    auto uiRenderers = entitiesRegistry->GetComponentIterator<UIRenderer>();

    // Opaque pass
    for (int i = 0; i < uiRenderers.Size(); ++i)
        if (!uiRenderers[i].IsTransparent())
            uiRenderers[i].OnRender(rtAccessor.Get(uiRenderers[i].Owner));

    // Transparent pass
    for (int i = uiRenderers.Size() - 1; i >= 0; --i)
        if (uiRenderers[i].IsTransparent())
            uiRenderers[i].OnRender(rtAccessor.Get(uiRenderers[i].Owner));
}
