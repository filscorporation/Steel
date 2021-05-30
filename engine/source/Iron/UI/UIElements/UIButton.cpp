#include "UIButton.h"
#include "../../Core/Log.h"
#include "../../Core/Time.h"
#include "../../Scene/SceneHelper.h"
#include "../../Scripting/ScriptingCore.h"
#include "../../Scripting/ScriptingSystem.h"

void UIButton::Init(UIEventHandler& eventHandler)
{
    eventHandler.EventCallback = UIButton::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                    | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    UIInteractable::Init(UpdateTransition);
}

void UIButton::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIButton>(handler).HandleEventInner(eventType, uiEvent);
}

bool UIButton::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UIButton>(entityID).UIInteractable::UpdateTransition();
}

void UIButton::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (!IsInteractable)
        return;

    if (eventType & UIEventTypes::MouseEnter)
    {
        IsHovered = true;
        PlayTransition(CurrentTransitionsInfo.Hovered);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        IsHovered = false;
        PlayTransition(CurrentTransitionsInfo.Normal);
    }
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        PlayTransition(CurrentTransitionsInfo.Clicked);
        if (Callback != nullptr)
            Callback(Owner);
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, CallbackTypes::ButtonClick, ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    if (eventType & UIEventTypes::MouseJustReleased)
    {
        PlayTransition(CurrentTransitionsInfo.Hovered);
    }
}
