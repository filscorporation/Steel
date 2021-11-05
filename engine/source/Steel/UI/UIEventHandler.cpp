#include "UIEventHandler.h"
#include "../Core/Log.h"
#include "../Core/Time.h"
#include "../Scripting/ScriptComponent.h"

void UIEventHandler::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
    {
        EnableNotifyScripts();
    }
}

void UIEventHandler::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
    {
        DisableNotifyScripts();
    }
}

void UIEventHandler::Update(EntitiesRegistry* entitiesRegistry, RectTransformation& rectTransformation)
{
    SortingOrder = entitiesRegistry->GetComponent<RectTransformation>(Owner).GetSortingOrder();
}

void UIEventHandler::HandleEvent(const ComponentAccessor<RectTransformation>& rtAccessor, UIEvent& uiEvent)
{
    auto& rectTransformation = rtAccessor.Get(RectEntity == NULL_ENTITY ? Owner : RectEntity);
    bool contains = rectTransformation.Contains(uiEvent.MousePosition);

    bool wasUsed = uiEvent.Used;
    switch (Type)
    {
        case EventHandlerTypes::Normal:
            uiEvent.Used = wasUsed || contains && uiEvent.ClippingDepth == 0;
            break;
        case EventHandlerTypes::Transparent:
            break;
        case EventHandlerTypes::ClippingOpen:
            if (!contains)
                uiEvent.ClippingDepth++;
            break;
        case EventHandlerTypes::ClippingClose:
            if (!contains)
                uiEvent.ClippingDepth--;
            break;
    }
    wasUsed = wasUsed || uiEvent.ClippingDepth != 0;

    if (Type == EventHandlerTypes::ClippingOpen || Type == EventHandlerTypes::ClippingClose
        || (EventCallback == nullptr && !notifyScripts))
        return;

    UIEventTypes::UIEventType eventsMask = EventsMask;
    if (notifyScripts)
        eventsMask = eventsMask | UIEventTypes::MouseEnter | UIEventTypes::MouseExit | UIEventTypes::MouseOver
                | UIEventTypes::MousePressed | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    auto eventType = (UIEventTypes::UIEventType)0;

    // TODO: after contains should cast to sprite image (to ignore transparent places)
    if (contains)
    {
        // Inner events - ones that only happen when pointer is inside UI element's rect

        if (!wasUsed && !lastFrameContains)
        {
            lastFrameContains = true;
            if (eventsMask & UIEventTypes::MouseEnter)
            {
                eventType = eventType | UIEventTypes::MouseEnter;
            }
        }
        if (eventsMask & UIEventTypes::MouseOver)
        {
            if (!wasUsed)
            {
                eventType = eventType | UIEventTypes::MouseOver;
            }
        }
        if (eventsMask & UIEventTypes::MouseJustPressed)
        {
            if (!wasUsed &&
                (uiEvent.LeftMouseButtonState == ButtonStates::JustPressed
                    || uiEvent.RightMouseButtonState == ButtonStates::JustPressed
                    || uiEvent.MiddleMouseButtonState == ButtonStates::JustPressed))
            {
                eventType = eventType | UIEventTypes::MouseJustPressed;
            }
        }
        if (eventsMask & UIEventTypes::MousePressed)
        {
            if (!wasUsed &&
                (uiEvent.LeftMouseButtonState == ButtonStates::IsHeld
                    || uiEvent.RightMouseButtonState == ButtonStates::IsHeld
                    || uiEvent.MiddleMouseButtonState == ButtonStates::IsHeld))
            {
                eventType = eventType | UIEventTypes::MousePressed;
            }
        }
        if (eventsMask & UIEventTypes::MouseJustReleased)
        {
            if (!wasUsed &&
                (uiEvent.LeftMouseButtonState == ButtonStates::JustReleased
                    || uiEvent.RightMouseButtonState == ButtonStates::JustReleased
                    || uiEvent.MiddleMouseButtonState == ButtonStates::JustReleased))
            {
                eventType = eventType | UIEventTypes::MouseJustReleased;
            }
        }
        if (!wasUsed && !isDragged && uiEvent.LeftMouseButtonState == ButtonStates::JustPressed)
        {
            isDragged = true;
            if (eventsMask & UIEventTypes::MouseDragBegin)
            {
                eventType = eventType | UIEventTypes::MouseDragBegin;
            }
        }
    }

    // Outside events - they do not require pointer being inside UI element rect
    if ((!contains || wasUsed) && lastFrameContains)
    {
        lastFrameContains = false;
        if (eventsMask & UIEventTypes::MouseExit)
        {
            eventType = eventType | UIEventTypes::MouseExit;
        }
    }
    if (isDragged && uiEvent.LeftMouseButtonState == ButtonStates::JustReleased)
    {
        isDragged = false;
        if (eventsMask & UIEventTypes::MouseDragEnd)
        {
            eventType = eventType | UIEventTypes::MouseDragEnd;
        }
    }
    if (isDragged && (eventsMask & UIEventTypes::MouseDrag))
    {
        eventType = eventType | UIEventTypes::MouseDrag;
    }
    if (eventsMask & UIEventTypes::ScrollDelta)
    {
        if (std::abs(uiEvent.ScrollDelta.x) > 0.001f && std::abs(uiEvent.ScrollDelta.y) > 0.001f)
        {
            eventType = eventType | UIEventTypes::ScrollDelta;
        }
    }
    if (eventsMask & UIEventTypes::MouseJustPressedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::JustPressed
            || uiEvent.RightMouseButtonState == ButtonStates::JustPressed
            || uiEvent.MiddleMouseButtonState == ButtonStates::JustPressed)
        {
            eventType = eventType | UIEventTypes::MouseJustPressedAnywhere;
        }
    }
    if (eventsMask & UIEventTypes::MousePressedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::IsHeld
            || uiEvent.RightMouseButtonState == ButtonStates::IsHeld
            || uiEvent.MiddleMouseButtonState == ButtonStates::IsHeld)
        {
            eventType = eventType | UIEventTypes::MousePressedAnywhere;
        }
    }
    if (eventsMask & UIEventTypes::MouseJustReleasedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::JustReleased
            || uiEvent.RightMouseButtonState == ButtonStates::JustReleased
            || uiEvent.MiddleMouseButtonState == ButtonStates::JustReleased)
        {
            eventType = eventType | UIEventTypes::MouseJustReleasedAnywhere;
        }
    }
    if (eventsMask & UIEventTypes::KeyInput && uiEvent.AnyKey)
    {
        eventType = eventType | UIEventTypes::KeyInput;
    }
    if (eventsMask & UIEventTypes::TextInput && !uiEvent.InputString.empty())
    {
        eventType = eventType | UIEventTypes::TextInput;
    }

    if (eventType != 0)
    {
        if (EventCallback != nullptr)
            EventCallback(Owner, eventType, uiEvent);
        ScriptsCallback(eventType, uiEvent);
    }
}

void UIEventHandler::EnableNotifyScripts()
{
    notifyScripts = true;
}

void UIEventHandler::DisableNotifyScripts()
{
    notifyScripts = false;
}

void UIEventHandler::ScriptsCallback(UIEventTypes::UIEventType eventType, const UIEvent& uiEvent)
{
    if (!notifyScripts)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (eventType & UIEventTypes::MouseEnter)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMouseEnterUI();
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMouseExitUI();
    }
    if (eventType & UIEventTypes::MouseOver)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMouseOverUI();
    }
    if (eventType & UIEventTypes::MousePressed)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMousePressedUI();
    }
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMouseJustPressedUI();
    }
    if (eventType & UIEventTypes::MouseJustReleased)
    {
        if (entitiesRegistry->HasComponent<ScriptComponent>(Owner))
            entitiesRegistry->GetComponent<ScriptComponent>(Owner).OnMouseJustReleasedUI();
    }
}
