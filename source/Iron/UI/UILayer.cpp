#include "UILayer.h"
#include "UIRenderer.h"
#include "UIEventHandler.h"
#include "UILetterRenderer.h"
#include "UIText.h"
#include "../Rendering/Renderer.h"
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

    // Rebuild text
    auto texts = entitiesRegistry->GetComponentIterator<UIText>();
    for (auto& text : texts)
    {
        text.Rebuild();
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
    auto rectTransformations = entitiesRegistry->GetComponentIterator<RectTransformation>();
    auto uiRenderers = entitiesRegistry->GetComponentIterator<UIRenderer>();
    auto textRenderers = entitiesRegistry->GetComponentIterator<UILetterRenderer>();

    Renderer::SetDrawMode(DrawModes::Normal);
    // Opaque pass
    for (int i = 0; i < uiRenderers.Size(); ++i)
        if (!uiRenderers[i].IsTransparent)
            uiRenderers[i].Render(rtAccessor.Get(uiRenderers[i].Owner));
    Renderer::EndBatch();

    Renderer::StartBatch();
    Renderer::SetDrawMode(DrawModes::Text);
    // Text letters
    for (int i = 0; i < textRenderers.Size(); ++i)
        textRenderers[i].Render();
    Renderer::EndBatch();

    Renderer::StartBatch();
    Renderer::SetDrawMode(DrawModes::Normal);
    // Transparent pass
    for (int i = uiRenderers.Size() - 1; i >= 0; --i)
        if (uiRenderers[i].IsTransparent)
            uiRenderers[i].Render(rtAccessor.Get(uiRenderers[i].Owner));
}

void UILayer::PollEvent(UIEvent& uiEvent)
{
    auto entitiesRegistry = _scene->GetEntitiesRegistry();
    entitiesRegistry->ApplyOrder<RectTransformation, UIEventHandler>();
    auto rectTransformations = entitiesRegistry->GetComponentIterator<RectTransformation>();
    auto uiEventHandlers = entitiesRegistry->GetComponentIterator<UIEventHandler>();

    for (int i = 0; i < uiEventHandlers.Size(); ++i)
        uiEventHandlers[i].HandleEvent(rectTransformations[i], uiEvent);

    _isPointerOverUI = uiEvent.Used;
}

bool UILayer::IsPointerOverUI()
{
    return _isPointerOverUI;
}
