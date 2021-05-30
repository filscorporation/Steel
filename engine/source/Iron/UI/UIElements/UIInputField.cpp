#include "UIInputField.h"
#include "../../Core/Input.h"
#include "../../Core/Log.h"
#include "../../Scene/SceneHelper.h"
#include "../../Scripting/ScriptingCore.h"
#include "../../Scripting/ScriptingSystem.h"

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
        Disselect(uiText.GetText());
    }
    if (IsSelected && eventType & UIEventTypes::TextInput)
    {
        wasEdited = true;
        uiText.SetText(uiText.GetText() + uiEvent.InputString);
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                           ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    if (IsSelected && eventType & UIEventTypes::KeyInput)
    {
        if (Input::IsKeyJustPressed(KeyCodes::Backspace))
        {
            auto text = uiText.GetText();
            if (!text.empty())
            {
                wasEdited = true;
                uiText.SetText(text.substr(0, text.size() - 1));
            }
            if (ScriptingSystem::IsInitialized())
                ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                               ScriptingCore::EventManagerCalls.callInvokeCallbacks);
        }
        if (Input::IsKeyJustPressed(KeyCodes::Enter))
            Disselect(uiText.GetText());
        if (Input::IsKeyJustPressed(KeyCodes::Escape))
            Disselect(uiText.GetText());
    }
}

bool UIInputField::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UIInputField>(entityID).UIInteractable::UpdateTransition();
}

void UIInputField::Disselect(std::string text)
{
    if (wasEdited)
    {
        wasEdited = false;
        if (SubmitCallback != nullptr)
            SubmitCallback(Owner, text);
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldEndEdit,
                                           ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    IsSelected = false;
    if (IsHovered)
        PlayTransition(CurrentTransitionsInfo.Hovered);
    else
        PlayTransition(CurrentTransitionsInfo.Normal);
}
