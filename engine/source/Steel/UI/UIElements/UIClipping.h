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

    VertexBuffer vb;
    IndexBuffer ib;
    float _sortingOrder = 0;
};
