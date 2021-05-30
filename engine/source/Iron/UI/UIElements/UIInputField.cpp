#include "UIInputField.h"
#include "../../Core/Input.h"
#include "../../Scene/SceneHelper.h"

void UIInputField::Init(UIEventHandler& eventHandler)
{
    eventHandler.EventCallback = UIInputField::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::TextInput | UIEventTypes::KeyInput |
            UIEventTypes::MouseEnter | UIEventTypes::MouseExit |
            UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustPressedAnywhere;

    UIInteractable::Init(UpdateTransition);
}

void UIInputField::SetTargetText(EntityID targetID)
{
    _targetText = targetID;
}

EntityID UIInputField::GetTargetText() const
{
    return _targetText;
}

void UIInputField::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent &uiEvent)
{
    GetComponentS<UIInputField>(handler).HandleEventInner(eventType, uiEvent);
}

void UIInputField::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (_targetText == NULL_ENTITY)
        return;

    auto& uiText = GetComponentS<UIText>(_targetText);
    if (eventType & UIEventTypes::MouseEnter)
    {
        IsHovered = true;
        if (!IsSelected)
            PlayTransition(CurrentTransitionsInfo.Hovered);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        IsHovered = false;
        if (IsSelected)
            PlayTransition(CurrentTransitionsInfo.Selected);
        else
            PlayTransition(CurrentTransitionsInfo.Normal);
    }
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        if (!IsSelected)
        {
            IsSelected = true;
            PlayTransition(CurrentTransitionsInfo.Selected);
        }
    }
    if (eventType & UIEventTypes::MouseJustPressedAnywhere && !(eventType & UIEventTypes::MouseJustPressed))
    {
        if (IsSelected)
        {
            IsSelected = false;
            PlayTransition(CurrentTransitionsInfo.Normal);
        }
    }
    if (IsSelected && eventType & UIEventTypes::TextInput)
    {
        uiText.SetText(uiText.GetText() + uiEvent.InputString);
    }
    if (IsSelected && eventType & UIEventTypes::KeyInput)
    {
        if (Input::IsKeyJustPressed(KeyCodes::Backspace))
        {
            auto text = uiText.GetText();
            if (!text.empty())
                uiText.SetText(text.substr(0, text.size() - 1));
        }
    }
}

bool UIInputField::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UIInputField>(entityID).UIInteractable::UpdateTransition();
}
