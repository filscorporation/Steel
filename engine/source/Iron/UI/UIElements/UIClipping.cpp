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
        qr.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
        qr.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
        qr.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
        qr.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
        qr.SetDefaultQuad();
        qr.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        qr.RenderMaterial = Application::Instance->GetResourcesManager()->DefaultUIClippingMaterial();
        qr.CustomOwner = Owner;
        // First two clipping quads for opaque pass, last ones for transparent
        qr.Queue = i > 1 ? RenderingQueue::Transparent : RenderingQueue::Opaque;
        // One clipping quad per pass increments stencil value, other - decrements
        StencilOperations::StencilOperation op = i == 0 || i == 2 ? StencilOperations::Increment : StencilOperations::Decrement;
        qr.CustomProperties.SetStencilOperation(StencilOperations::Keep, op, op);
    }

    needRebuild = true;

    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));
}

void UIClipping::Rebuild(UILayer* layer, RectTransformation& transformation, bool sortingOrderDirty)
{
    if (!needRebuild && !transformation.DidTransformationChange() && !sortingOrderDirty)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    float sortingOrder = transformation.GetSortingOrder();
    float dz = 1.0f / (float)layer->GetLayerThickness();
    float thickness = (float) transformation.GetChildrenThickness();

    for (int i = 0; i < 4; ++i)
    {
        auto& qr = registry->GetComponent<UIQuadRenderer>(clippingQuads[i]);
        for (int j = 0; j < 4; ++j)
            qr.Vertices[j] = matrix * qr.DefaultVertices[j];
        qr.SortingOrder = i == 0 || i == 3 ? sortingOrder - dz * SO_OFFSET : sortingOrder + dz * (thickness + SO_OFFSET);
    }

    needRebuild = false;

    // TODO: event handling rects
    //if (registry->HasComponent<UIEventHandler>(Owner))
    //    registry->GetComponent<UIEventHandler>(Owner).SortingOrder = sortingOrder;
}

void UIClipping::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : clippingQuads)
        entitiesRegistry->DeleteEntity(qrID);
    clippingQuads.clear();

    wasRemoved = true;
    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));
}

void UIClipping::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    Init(entitiesRegistry);
}

void UIClipping::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : clippingQuads)
        entitiesRegistry->DeleteEntity(qrID);
    clippingQuads.clear();
}

bool UIClipping::WasRemoved() const
{
    return wasRemoved;
}
