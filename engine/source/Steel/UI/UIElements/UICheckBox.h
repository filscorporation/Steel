#pragma once

#include <functional>

#include "UIImage.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/UI/UIEvent.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/UI/UIInteractable.h"
#include "Steel/UI/TransitionInfo.h"

typedef std::function<void(EntityID)> CheckBoxCallback;

class UICheckBox : public UIInteractable
{
    COMPONENT(UICheckBox)

public:
    explicit UICheckBox(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    bool GetValue() const;
    void SetValue(bool value);

    CheckBoxCallback Callback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);

    bool isChecked = false;
    EntityID checkMark = NULL_ENTITY;

    friend class UILayer;
};
