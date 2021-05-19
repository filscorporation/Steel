#pragma once

#include "../EntityComponentSystem/Component.h"
#include "UIEvent.h"
#include "RectTransformation.h"

using EventCallbackFunction = void (*)(EntityID, UIEventTypes::UIEventType, UIEvent&);

class UIEventHandler : public Component
{
public:
    explicit UIEventHandler(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void HandleEvent(const RectTransformation& rectTransformation, UIEvent& uiEvent);

    // UI element defined parameters
    bool IsTransparent = false;
    float SortingOrder = 0;
    UIEventTypes::UIEventType EventsMask = (UIEventTypes::UIEventType)0;
    EventCallbackFunction EventCallback = nullptr;

private:
    bool lastFrameContains = false;
    bool isDragged = false;
};
