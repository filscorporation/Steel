#pragma once

#include <functional>

#include "UIImage.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"
#include "../UIInteractable.h"
#include "../TransitionInfo.h"
#include "../../Rendering/Sprite.h"

typedef std::function<void(EntityID)> ButtonCallback;

class UIButton : public UIInteractable
{
public:
    explicit UIButton(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { };

    void Init(UIEventHandler& eventHandler);

    ButtonCallback Callback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);

    friend class UISystem;
};
