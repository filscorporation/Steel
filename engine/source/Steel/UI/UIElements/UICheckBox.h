#pragma once

#include <functional>

#include "UIImage.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"
#include "../UIInteractable.h"
#include "../TransitionInfo.h"
#include "../../Rendering/Sprite.h"

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

    friend class UISystem;
    friend class UILayer;
};
