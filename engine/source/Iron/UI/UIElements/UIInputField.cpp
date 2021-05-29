#include "UIInputField.h"
#include "../../Core/Input.h"
#include "../../Scene/SceneHelper.h"

void UIInputField::Init(UIEventHandler& eventHandler)
{
    eventHandler.EventCallback = UIInputField::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::TextInput | UIEventTypes::KeyInput |
            UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustPressedAnywhere;
}

void UIInputField::SetTargetImage(EntityID targetID)
{
    _targetImage = targetID;
}

EntityID UIInputField::GetTargetImage() const
{
    return _targetImage;
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
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        if (!isFocused)
            isFocused = true;
    }
    if (eventType & UIEventTypes::MouseJustPressedAnywhere && !(eventType & UIEventTypes::MouseJustPressed))
    {
        if (isFocused)
            isFocused = false;
    }
    if (isFocused && eventType & UIEventTypes::TextInput)
    {
        uiText.SetText(uiText.GetText() + uiEvent.InputString);
    }
    if (isFocused && eventType & UIEventTypes::KeyInput)
    {
        if (Input::IsKeyJustPressed(KeyCodes::Backspace))
        {
            auto text = uiText.GetText();
            if (!text.empty())
                uiText.SetText(text.substr(0, text.size() - 1));
        }
    }
}
