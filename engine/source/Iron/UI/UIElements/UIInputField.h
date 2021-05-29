#pragma once

#include "../UIComponent.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"

class UIInputField : public UIComponent
{
public:
    explicit UIInputField(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    void Init(UIEventHandler& eventHandler);

    void SetTargetImage(EntityID targetID);
    EntityID GetTargetImage() const;
    void SetTargetText(EntityID targetID);
    EntityID GetTargetText() const;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);

    EntityID _targetImage = NULL_ENTITY;
    EntityID _targetText = NULL_ENTITY;
    bool isFocused = false;
};
