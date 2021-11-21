#pragma once

#include "../UIComponent.h"
#include "../RectTransformation.h"
#include "../../EntityComponentSystem/EntitiesRegistry.h"

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

    bool WasRemoved() const;

    short ClippingLevel = 1;

private:
    std::vector<EntityID> clippingQuads;
    EntityID openingEH = NULL_ENTITY;
    EntityID closingEH = NULL_ENTITY;
    bool needRebuild = false;
    // This is used to prevent clippling recalculation process initiated by this element to count it in
    bool wasRemoved = false;

    void InitCaps(EntitiesRegistry* entitiesRegistry);
    void ClearCaps(EntitiesRegistry* entitiesRegistry);
};
