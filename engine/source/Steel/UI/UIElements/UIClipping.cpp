#include "UIClipping.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/Hierarchy.h"
#include "Steel/UI/UIEventHandler.h"

#define SO_OFFSET 0.1f

bool UIClipping::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIClipping::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    InitCaps(entitiesRegistry);
}

void UIClipping::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    for (int i = 0; i < 4; ++i)
    {
        vb[i].Clear();
        ib[i].Clear();
    }

    if (entitiesRegistry->IsCleared())
        return;

    ClearCaps(entitiesRegistry);

    wasRemoved = true;
    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));
}

void UIClipping::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    InitCaps(entitiesRegistry);
}

void UIClipping::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    ClearCaps(entitiesRegistry);
}

void UIClipping::InitCaps(EntitiesRegistry* entitiesRegistry)
{
    ClippingLevel = GetClippingLevelUpwards(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode());
    ClippingLevel++;

    _customProperties[0].SetStencilOperation(StencilOperations::Keep, StencilOperations::Increment, StencilOperations::Increment);
    _customProperties[1].SetStencilOperation(StencilOperations::Keep, StencilOperations::Decrement, StencilOperations::Decrement);
    // Clipping planes should not write to depth buffer
    _customProperties[0].SetDepthMask(false);
    _customProperties[1].SetDepthMask(false);

    // Init event handlers to discard UI events outside of clipping area
    openingEH = entitiesRegistry->CreateNewEntity();
    auto& eh1 = entitiesRegistry->AddComponent<UIEventHandler>(openingEH);
    eh1.Type = EventHandlerTypes::ClippingOpen;
    eh1.RectEntity = Owner;
    closingEH = entitiesRegistry->CreateNewEntity();
    auto& eh2 = entitiesRegistry->AddComponent<UIEventHandler>(closingEH);
    eh2.Type = EventHandlerTypes::ClippingClose;
    eh2.RectEntity = Owner;

    UpdateHierarchyDependantProperties(entitiesRegistry, entitiesRegistry->GetComponent<HierarchyNode>(Owner));

    isDirty = true;
}

void UIClipping::Rebuild(UILayer* layer, RectTransformation& transformation, bool sortingOrderDirty)
{
    if (!isDirty && !transformation.DidTransformationChange() && !sortingOrderDirty)
        return;

    RebuildInner(transformation);

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    // Opening event handler cap
    entitiesRegistry->GetComponent<UIEventHandler>(openingEH).SortingOrder = _sortingOrder[0];
    // Closing event handler cap
    entitiesRegistry->GetComponent<UIEventHandler>(closingEH).SortingOrder = _sortingOrder[1];
}

void UIClipping::Draw(RenderContext* renderContext)
{
    if (vb[0].IsEmpty() || ib[0].IsEmpty() || vb[1].IsEmpty() || ib[1].IsEmpty())
        return;

    for (int i = 0; i < 4; ++i)
    {
        DrawCall drawCall;
        drawCall.VB = vb[i];
        drawCall.IB = ib[i];
        drawCall.RenderMaterial = Application::Instance->GetResourcesManager()->DefaultUIClippingMaterial();
        drawCall.CustomProperties = i == 0 || i == 2 ? _customProperties[0] : _customProperties[1];
        drawCall.SortingOrder = i == 0 || i == 3 ? _sortingOrder[0] : _sortingOrder[1];
        drawCall.Queue = i < 2 ? RenderingQueue::Opaque : RenderingQueue::Transparent;

        renderContext->List.AddDrawCall(drawCall);
    }
}

void UIClipping::RebuildInner(RectTransformation& transformation)
{
    isDirty = false;

    UILayer* layer = Application::Instance->GetCurrentScene()->GetUILayer();
    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    float sortingOrder = transformation.GetSortingOrder();
    float dz = 1.0f / (float)layer->GetLayerThickness();
    float thickness = transformation.GetChildrenThickness();

    std::vector<VertexAttribute> attributes;
    attributes.reserve(1);
    attributes.emplace_back(0, 3);

    for (int i = 0; i < 4; ++i)
    {
        const uint32_t verticesSize = 9 * 4;
        auto vertices = new float[verticesSize];

        glm::vec3 verticesVectors[4];
        verticesVectors[0] = matrix * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        verticesVectors[1] = matrix * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
        verticesVectors[2] = matrix * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
        verticesVectors[3] = matrix * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);

        uint32_t offset = 0;
        for (auto& verticesVector : verticesVectors)
        {
            vertices[offset++] = verticesVector[0];
            vertices[offset++] = verticesVector[1];
            vertices[offset++] = verticesVector[2];
        }

        auto indices = new uint32_t[6]{ 0, 1, 2, 1, 2, 3 };

        vb[i].Create(vertices, verticesSize, attributes);
        ib[i].Create(indices, 6);
    }

    _sortingOrder[0] = sortingOrder - dz * (thickness + SO_OFFSET);
    _sortingOrder[1] = sortingOrder + dz * SO_OFFSET;
}

bool UIClipping::WasRemoved() const
{
    return wasRemoved;
}

void UIClipping::ClearCaps(EntitiesRegistry* entitiesRegistry)
{
    entitiesRegistry->DeleteEntity(openingEH);
    openingEH = NULL_ENTITY;
    entitiesRegistry->DeleteEntity(closingEH);
    closingEH = NULL_ENTITY;
}
