#pragma once

#include "Steel/UI/UIComponent.h"
#include "Steel/UI/RectTransformation.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"

class UIScrollableView : public UIComponent
{
    DEFINE_TYPE(UIScrollableView)

public:
    explicit UIScrollableView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;

    EntityID GetContent() const;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);

    void SetContent(EntityID contentEntityID);

    EntityID content = NULL_ENTITY;
};
