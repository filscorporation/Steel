#pragma once

#include "../UIComponent.h"
#include "../RectTransformation.h"
#include "../../EntityComponentSystem/EntitiesRegistry.h"

class UIClipping : public UIComponent
{
public:
    explicit UIClipping(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry);
    void Rebuild(UILayer *layer, RectTransformation &transformation, bool sortingOrderDirty);

    void OnRemoved(EntitiesRegistry* entitiesRegistry);
    void OnEnabled(EntitiesRegistry* entitiesRegistry);
    void OnDisabled(EntitiesRegistry* entitiesRegistry);

    bool WasRemoved() const;

    short ClippingLevel = 1;

private:
    std::vector<EntityID> clippingQuads;
    bool needRebuild = false;
    // This is used to prevent clippling recalculation process initiated by this element to count it in
    bool wasRemoved = false;
};
