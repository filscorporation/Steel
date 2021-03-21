#pragma once

#include "UIComponent.h"
#include "UIRenderer.h"
#include "UIEvent.h"
#include "UIImage.h"
#include "../Rendering/Sprite.h"

class UIButton : public UIImage
{
public:
    explicit UIButton(EntityID ownerEntityID);

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
};
