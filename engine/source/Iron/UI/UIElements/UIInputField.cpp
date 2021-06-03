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
            Select(uiText);
        else
        {
            uint32_t newPosition = uiText.GetCursorPosition(uiEvent.MousePosition);
            if (newPosition != cursorPosition)
            {
                cursorPosition = newPosition;
                uiText.SetCursorPosition(cursorPosition);
            }
        }
    }
    if (eventType & UIEventTypes::MouseJustPressedAnywhere && !(eventType & UIEventTypes::MouseJustPressed))
    {
        Disselect(uiText);
    }
    if (IsSelected && eventType & UIEventTypes::TextInput)
    {
        AddText(uiText, uiEvent.InputString);
    }
    if (IsSelected && eventType & UIEventTypes::KeyInput)
    {
        if (Input::IsKeyJustPressed(KeyCodes::Backspace))
        {
            if (cursorPosition > 0 && !uiText.GetText().empty())
            {
                wasEdited = true;
                uiText.SetText(uiText.GetText().erase(cursorPosition - 1, 1));
                cursorPosition--;
                uiText.SetCursorPosition(cursorPosition);
                if (ScriptingSystem::IsInitialized())
                    ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                                   ScriptingCore::EventManagerCalls.callInvokeCallbacks);
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Delete))
        {
            if (cursorPosition < uiText.GetText().size() && !uiText.GetText().empty())
            {
                wasEdited = true;
                uiText.SetText(uiText.GetText().erase(cursorPosition, 1));
                if (ScriptingSystem::IsInitialized())
                    ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                                   ScriptingCore::EventManagerCalls.callInvokeCallbacks);
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Enter))
        {
            AddText(uiText, "\n");
        }
        if (Input::IsKeyJustPressed(KeyCodes::Escape))
        {
            Disselect(uiText);
        }
        if (Input::IsKeyJustPressed(KeyCodes::Left))
        {
            if (cursorPosition != 0)
            {
                cursorPosition = std::min(uiText.GetText().size(), cursorPosition) - 1;
                uiText.SetCursorPosition(cursorPosition);
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Right))
        {
            if (cursorPosition < uiText.GetText().size())
            {
                cursorPosition = cursorPosition + 1;
                uiText.SetCursorPosition(cursorPosition);
            }
        }
    }
}

bool UIInputField::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UIInputField>(entityID).UIInteractable::UpdateTransition();
}

void UIInputField::Select(UIText& uiText)
{
    IsSelected = true;
    cursorPosition = uiText.GetText().size();
    uiText.SetCursorPosition(cursorPosition);
    PlayTransition(CurrentTransitionsInfo.Selected);
}

void UIInputField::Disselect(UIText& uiText)
{
    if (wasEdited)
    {
        wasEdited = false;
        if (SubmitCallback != nullptr)
            SubmitCallback(Owner, uiText.GetText());
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldEndEdit,
                                           ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    IsSelected = false;
    uiText.DisableCursor();
    if (IsHovered)
        PlayTransition(CurrentTransitionsInfo.Hovered);
    else
        PlayTransition(CurrentTransitionsInfo.Normal);
}

void UIInputField::AddText(UIText& uiText, const std::string& text)
{
    wasEdited = true;
    std::string newText = uiText.GetText();
    uint32_t offset = cursorPosition;
    newText.insert(offset, text);
    uiText.SetText(newText);
    cursorPosition += text.size();
    uiText.SetCursorPosition(cursorPosition);

    if (ScriptingSystem::IsInitialized())
        ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                       ScriptingCore::EventManagerCalls.callInvokeCallbacks);
}
