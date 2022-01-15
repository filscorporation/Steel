#pragma once

#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/Rendering/Core/IndexBuffer.h"
#include "Steel/Rendering/Core/RenderContext.h"
#include "Steel/Rendering/Core/VertexBuffer.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/RectTransformation.h"

class UIClipping : public UIComponent
{
    COMPONENT(UIClipping)

public:
    explicit UIClipping(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(UILayer* layer, RectTransformation& transformation, bool sortingOrderDirty);
    void Draw(RenderContext* renderContext);

    bool WasRemoved() const;

    short ClippingLevel = 1;

private:
    void RebuildInner(RectTransformation& transformation);
    void InitCaps(EntitiesRegistry* entitiesRegistry);
    void ClearCaps(EntitiesRegistry* entitiesRegistry);

    EntityID openingEH = NULL_ENTITY;
    EntityID closingEH = NULL_ENTITY;
    // This is used to prevent clipping recalculation process initiated by this element to count it in
    bool wasRemoved = false;
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties[2];

    bool isDirty = true;
    VertexBuffer vb[4];
    IndexBuffer ib[4];
    float _sortingOrder[2] = { 0.0f, 0.0f };
};
