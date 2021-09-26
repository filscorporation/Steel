#pragma once

#include "../EntityComponentSystem/Component.h"
#include "UIEvent.h"
#include "RectTransformation.h"

using EventCallbackFunction = void (*)(EntityID, UIEventTypes::UIEventType, UIEvent&);

namespace EventHandlerTypes
{
    enum EventHandlerType
    {
        Normal,
        Transparent,
        ClippingOpen,
        ClippingClose,
    };
}

class UIEventHandler : public Component
{
public:
    explicit UIEventHandler(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Update(EntitiesRegistry* entitiesRegistry, RectTransformation& rectTransformation);
    void HandleEvent(const ComponentAccessor<RectTransformation>& rtAccessor, UIEvent& uiEvent);

    void EnableNotifyScripts();
    void DisableNotifyScripts();

    // Parameters, defined by UI element
    EventHandlerTypes::EventHandlerType Type = EventHandlerTypes::Normal;
    EntityID RectEntity = NULL_ENTITY;
    float SortingOrder = 0;
    UIEventTypes::UIEventType EventsMask = (UIEventTypes::UIEventType)0;
    EventCallbackFunction EventCallback = nullptr;

private:
    bool lastFrameContains = false;
    bool isDragged = false;

    bool notifyScripts = false;

    void ScriptsCallback(UIEventTypes::UIEventType eventType, const UIEvent& uiEvent);
};
