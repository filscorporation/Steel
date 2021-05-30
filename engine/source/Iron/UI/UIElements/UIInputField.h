#pragma once

#include "../UIComponent.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"
#include "../UIInteractable.h"

typedef std::function<void(EntityID, std::string)> InputFieldSubmitCallback;

class UIInputField : public UIInteractable
{
public:
    explicit UIInputField(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { }

    void Init(UIEventHandler& eventHandler);

    void SetTargetText(EntityID targetID);
    EntityID GetTargetText() const;

    InputFieldSubmitCallback SubmitCallback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);

    void Disselect(std::string text);

    EntityID _targetText = NULL_ENTITY;
    bool wasEdited = false;
};
