#include "UIClipping.h"
#include "../UIQuadRenderer.h"
#include "../UIEventHandler.h"
#include "../../Core/Application.h"
#include "../../Scene/Hierarchy.h"

#define SO_OFFSET 0.1f

void UIClipping::Init(EntitiesRegistry* entitiesRegistry)
{
    ClippingLevel = GetClippingLevelUpwards(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner).ParentNode);
    ClippingLevel++;

    clippingQuads.reserve(4);
    for (int i = 0; i < 4; ++i)
    {
        clippingQuads.push_back(entitiesRegistry->CreateNewEntity());
        auto& qr = entitiesRegistry->AddComponent<UIQuadRenderer>(clippingQuads[i]);
        qr.SetDefaultQuad();
        qr.RenderMaterial = Application::Instance->GetResourcesManager()->DefaultUIClippingMaterial();
        qr.CustomOwner = Owner;
        // First two clipping quads for opaque pass, last ones for transparent
        qr.Queue = i > 1 ? RenderingQueue::Transparent : RenderingQueue::Opaque;
        // One clipping quad per pass increments stencil value, other - decrements
        StencilOperations::StencilOperation op = i == 0 || i == 2 ? StencilOperations::Increment : StencilOperations::Decrement;
        qr.CustomProperties.SetStencilOperation(StencilOperations::Keep, op, op);
    }

    openingEH = entitiesRegistry->CreateNewEntity();
    auto& eh1 = entitiesRegistry->AddComponent<UIEventHandler>(openingEH);
    eh1.Type = EventHandlerTypes::ClippingOpen;
    eh1.RectEntity = Owner;
    closingEH = entitiesRegistry->CreateNewEntity();
    auto& eh2 = entitiesRegistry->AddComponent<UIEventHandler>(closingEH);
    eh2.Type = EventHandlerTypes::ClippingClose;
    eh2.RectEntity = Owner;

    needRebuild = true;

    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));
}

void UIClipping::Rebuild(UILayer* layer, RectTransformation& transformation, bool sortingOrderDirty)
{
    if (!needRebuild && !transformation.DidTransformationChange() && !sortingOrderDirty)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    float sortingOrder = transformation.GetSortingOrder();
    float dz = 1.0f / (float)layer->GetLayerThickness();
    float thickness = (float)transformation.GetChildrenThickness();

    for (int i = 0; i < 4; ++i)
    {
        auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(clippingQuads[i]);
        for (int j = 0; j < 4; ++j)
            qr.Vertices[j] = matrix * qr.DefaultVertices[j];
        qr.SortingOrder = i == 0 || i == 3 ? sortingOrder - dz * SO_OFFSET : sortingOrder + dz * (thickness + SO_OFFSET);
    }

    needRebuild = false;

    // Opening event handler cap
    entitiesRegistry->GetComponent<UIEventHandler>(openingEH).SortingOrder = sortingOrder - dz * SO_OFFSET;
    // Closing event handler cap
    entitiesRegistry->GetComponent<UIEventHandler>(closingEH).SortingOrder = sortingOrder + dz * (thickness + SO_OFFSET);
}

void UIClipping::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    ClearCaps(entitiesRegistry);

    wasRemoved = true;
    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));
}

void UIClipping::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    Init(entitiesRegistry);
}

void UIClipping::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    ClearCaps(entitiesRegistry);
}

bool UIClipping::WasRemoved() const
{
    return wasRemoved;
}

void UIClipping::ClearCaps(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : clippingQuads)
        entitiesRegistry->DeleteEntity(qrID);
    clippingQuads.clear();
    entitiesRegistry->DeleteEntity(openingEH);
    openingEH = NULL_ENTITY;
    entitiesRegistry->DeleteEntity(closingEH);
    closingEH = NULL_ENTITY;
}
