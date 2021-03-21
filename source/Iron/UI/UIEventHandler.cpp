#include "UIEventHandler.h"
#include "../Core/Time.h"

void UIEventHandler::HandleEvent(const RectTransformation& rectTransformation, UIEvent& uiEvent)
{
    bool contains = rectTransformation.Contains(uiEvent.MousePosition);
    bool used = uiEvent.Used || contains && !IsTransparent;

    if (EventCallback == nullptr)
    {
        uiEvent.Used = used;
        return;
    }

    if (EventsMask & UIEventTypes::Any)
    {
        EventCallback(Owner, UIEventTypes::Any, uiEvent);
        uiEvent.Used = used;
        return;
    }

    auto eventType = (UIEventTypes::UIEventType)0;

    // TODO: after contains should cast to sprite image (to ignore transparent places)
    if (contains)
    {
        // Inner events - ones that only happen when pointer is inside UI element's rect

        if (EventsMask & UIEventTypes::MouseEnter)
        {
            if (!uiEvent.Used && !lastFrameContains)
            {
                lastFrameContains = true;
                eventType = eventType | UIEventTypes::MouseEnter;
            }
        }
        if (EventsMask & UIEventTypes::MouseOver)
        {
            if (!uiEvent.Used)
            {
                eventType = eventType | UIEventTypes::MouseOver;
            }
        }
        if (EventsMask & UIEventTypes::MouseJustPressed)
        {
            if (!uiEvent.Used &&
                (uiEvent.LeftMouseButtonState == ButtonStates::JustPressed || uiEvent.RightMouseButtonState == ButtonStates::JustPressed))
            {
                eventType = eventType | UIEventTypes::MouseJustPressed;
            }
        }
        if (EventsMask & UIEventTypes::MousePressed)
        {
            if (!uiEvent.Used &&
                (uiEvent.LeftMouseButtonState == ButtonStates::IsHeld || uiEvent.RightMouseButtonState == ButtonStates::IsHeld))
            {
                eventType = eventType | UIEventTypes::MousePressed;
            }
        }
        if (EventsMask & UIEventTypes::MouseJustReleased)
        {
            if (!uiEvent.Used &&
                (uiEvent.LeftMouseButtonState == ButtonStates::JustReleased || uiEvent.RightMouseButtonState == ButtonStates::JustReleased))
            {
                eventType = eventType | UIEventTypes::MouseJustReleased;
            }
        }
        if (!uiEvent.Used && !isDragged && uiEvent.LeftMouseButtonState == ButtonStates::JustPressed)
        {
            isDragged = true;
            if (EventsMask & UIEventTypes::MouseDragBegin)
            {
                eventType = eventType | UIEventTypes::MouseDragBegin;
            }
        }
    }

    // Outside events - they do not require pointer being inside UI element rect
    if ((!contains || uiEvent.Used) && lastFrameContains)
    {
        lastFrameContains = false;
        if (EventsMask & UIEventTypes::MouseExit)
        {
            eventType = eventType | UIEventTypes::MouseExit;
        }
    }
    if (isDragged && uiEvent.LeftMouseButtonState == ButtonStates::JustReleased)
    {
        isDragged = false;
        if (EventsMask & UIEventTypes::MouseDragEnd)
        {
            eventType = eventType | UIEventTypes::MouseDragEnd;
        }
    }
    if (isDragged && (EventsMask & UIEventTypes::MouseDrag))
    {
        eventType = eventType | UIEventTypes::MouseDrag;
    }
    if (EventsMask & UIEventTypes::ScrollDelta)
    {
        if (std::abs(uiEvent.ScrollDelta.x) > 0.001f && std::abs(uiEvent.ScrollDelta.y) > 0.001f)
        {
            eventType = eventType | UIEventTypes::ScrollDelta;
        }
    }
    if (EventsMask & UIEventTypes::MouseJustPressedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::JustPressed || uiEvent.RightMouseButtonState == ButtonStates::JustPressed)
        {
            eventType = eventType | UIEventTypes::MouseJustPressedAnywhere;
        }
    }
    if (EventsMask & UIEventTypes::MousePressedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::IsHeld || uiEvent.RightMouseButtonState == ButtonStates::IsHeld)
        {
            eventType = eventType | UIEventTypes::MousePressedAnywhere;
        }
    }
    if (EventsMask & UIEventTypes::MouseJustReleasedAnywhere)
    {
        if (uiEvent.LeftMouseButtonState == ButtonStates::JustReleased || uiEvent.RightMouseButtonState == ButtonStates::JustReleased)
        {
            eventType = eventType | UIEventTypes::MouseJustReleasedAnywhere;
        }
    }

    if (eventType != 0)
        EventCallback(Owner, eventType, uiEvent);

    uiEvent.Used = used;
}
