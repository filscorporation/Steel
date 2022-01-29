#pragma once

#include <functional>

#include "UIImage.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/UI/UIEvent.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/UI/UIInteractable.h"
#include "Steel/UI/TransitionInfo.h"

typedef std::function<void(EntityID)> ButtonCallback;

class UIButton : public UIInteractable
{
    DEFINE_COMPONENT(UIButton)

public:
    explicit UIButton(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    ButtonCallback Callback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);
};
