#include <algorithm>
#include <string>
#include <sstream>

#include "UIInputField.h"
#include "../../Core/Input.h"
#include "../../Core/Log.h"
#include "../../Core/Time.h"
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

void UIInputField::Rebuild(RectTransformation& transformation)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
    {
        if (drawCursor)
        {
            entitiesRegistry->EntitySetActive(cursor, false, true);
            drawCursor = false;
        }
        return;
    }

    if (drawCursor)
        UpdateCursorBlink();

    auto& uiText = GetComponentS<UIText>(_targetText);
    auto& uiTextRT = GetComponentS<RectTransformation>(_targetText);
    if (uiText.IsTextColorDirty() || cursorColorDirty)
    {
        UpdateCursorColor(uiText);
    }
    if (drawCursor && uiText.IsTextDirty())
    {
        SetCursorPosition(std::min(uiText.GetText().size(), cursorPosition));
    }
    if (uiTextRT.DidTransformationChange() || uiText.IsTextDirty()
        || transformation.DidTransformationChange() || cursorDirty)
    {
        RebuildCursor(uiText, uiTextRT);
    }

    cursorDirty = false;
    cursorColorDirty = false;
}

void UIInputField::SetTargetText(EntityID targetID)
{
    _targetText = targetID;
}

EntityID UIInputField::GetTargetText() const
{
    return _targetText;
}

void UIInputField::SetCursorWidth(uint32_t width)
{
    cursorWidth = width;
    cursorDirty = true;
}

uint32_t UIInputField::GetCursorWidth() const
{
    return cursorWidth;
}

void UIInputField::SetCursorColor(glm::vec4 color)
{
    cursorColor = color;
    autoCursorColor = false;
    cursorColorDirty = true;
}

glm::vec4 UIInputField::GetCursorColor() const
{
    return cursorColor;
}

void UIInputField::SetCursorAutoColor(bool isAuto)
{
    autoCursorColor = isAuto;
    cursorColorDirty = true;
}

bool UIInputField::GetCursorAutoColor() const
{
    return autoCursorColor;
}

void UIInputField::SetIsMultiline(bool isMultiline)
{
    if (multiline == isMultiline)
        return;

    multiline = isMultiline;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
        return;
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    std::string text = uiText.GetText();
    Validate(uiText.GetText(), text, false);
    uiText.SetText(text);
}

bool UIInputField::GetIsMultiline() const
{
    return multiline;
}

void UIInputField::SetTextType(TextTypes::TextType type)
{
    if (type == textType)
        return;

    textType = type;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
        return;
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    std::string text = uiText.GetText();
    Validate(uiText.GetText(), text, false);
    uiText.SetText(text);
}

TextTypes::TextType UIInputField::GetTextType() const
{
    return textType;
}

void UIInputField::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIInputField>(handler).HandleEventInner(eventType, uiEvent);
}

void UIInputField::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (!IsInteractable)
        return;

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

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
        return;
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);

    if (eventType & UIEventTypes::MouseJustPressed)
    {
        if (!IsSelected)
        {
            Select(uiText);
        }
        else
        {
            uint32_t newPosition = uiText.GetLetterPosition(uiEvent.MousePosition);
            cursorHorizontalOffset = -1;
            SetCursorPosition(newPosition);
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
                int diff = SetText(uiText, uiText.GetText().erase(cursorPosition - 1, 1));
                SetCursorPosition(cursorPosition + diff);
                cursorHorizontalOffset = -1;
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Delete))
        {
            if (cursorPosition < uiText.GetText().size() && !uiText.GetText().empty())
            {
                SetText(uiText, uiText.GetText().erase(cursorPosition, 1));
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Enter))
        {
            if (multiline)
                AddText(uiText, "\n");
            else
                Disselect(uiText);
        }
        if (Input::IsKeyJustPressed(KeyCodes::Escape))
        {
            Disselect(uiText);
        }
        if (Input::IsKeyJustPressed(KeyCodes::Left))
        {
            if (cursorPosition != 0)
            {
                SetCursorPosition(std::min(uiText.GetText().size(), cursorPosition) - 1);
                cursorHorizontalOffset = -1;
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Right))
        {
            if (cursorPosition < uiText.GetText().size())
            {
                SetCursorPosition(cursorPosition + 1);
                cursorHorizontalOffset = -1;
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Up))
        {
            uint32_t newPosition = uiText.GetLetterPositionLineUp(cursorPosition, cursorHorizontalOffset);
            SetCursorPosition(newPosition);
        }
        if (Input::IsKeyJustPressed(KeyCodes::Down))
        {
            uint32_t newPosition = uiText.GetLetterPositionLineDown(cursorPosition, cursorHorizontalOffset);
            SetCursorPosition(newPosition);
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
    SetCursorPosition(uiText.GetText().size());
    cursorHorizontalOffset = -1;
    PlayTransition(CurrentTransitionsInfo.Selected);
}

void UIInputField::Disselect(UIText& uiText)
{
    cursorPosition = -1;
    if (wasEdited)
    {
        std::string text = uiText.GetText();
        Validate(uiText.GetText(), text, false);
        uiText.SetText(text);
        wasEdited = false;
        if (SubmitCallback != nullptr)
            SubmitCallback(Owner, uiText.GetText());
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldEndEdit,
                                           ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    IsSelected = false;
    DisableCursor();
    if (IsHovered)
        PlayTransition(CurrentTransitionsInfo.Hovered);
    else
        PlayTransition(CurrentTransitionsInfo.Normal);
}

void UIInputField::AddText(UIText& uiText, const std::string& text)
{
    std::string newText = uiText.GetText();
    uint32_t offset = std::min(cursorPosition, newText.size());
    newText.insert(offset, text);

    int diff = SetText(uiText, newText);
    SetCursorPosition(cursorPosition + diff);
    cursorHorizontalOffset = -1;
}

int UIInputField::SetText(UIText& uiText, std::string& text)
{
    Validate(uiText.GetText(), text, false);
    int diff = (int)text.size() - (int)(uiText.GetText().size());
    uiText.SetText(text);
    wasEdited = true;

    if (ScriptingSystem::IsInitialized())
        ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                       ScriptingCore::EventManagerCalls.callInvokeCallbacks);

    return diff;
}

bool IsInt(const std::string& text, int& value)
{
    std::istringstream iss(text);
    iss >> std::noskipws >> value;
    return iss.eof() && !iss.fail();
}

bool IsFloat(const std::string& text, float& value)
{
    std::istringstream iss(text);
    iss >> std::noskipws >> value;
    return iss.eof() && !iss.fail();
}

void UIInputField::Validate(const std::string& oldText, std::string& newText, bool submit)
{
    if (!multiline)
    {
        newText.erase(std::remove(newText.begin(), newText.end(), '\n'), newText.end());
    }

    switch (textType)
    {
        case TextTypes::Standard:
            break;
        case TextTypes::IntegerNumber:
            int integer;
            if (!IsInt(newText, integer))
                newText = oldText;
            else if (submit)
                newText = std::to_string(integer);
            break;
        case TextTypes::DecimalNumber:
            float floatValue;
            if (!IsFloat(newText, floatValue))
                newText = oldText;
            else if (submit)
                newText = std::to_string(floatValue);
            break;
        case TextTypes::Alphanumeric:
            newText.erase(std::remove_if(newText.begin(), newText.end(), std::not1(std::ptr_fun(std::isalnum))), newText.end());
            break;
    }
}

void UIInputField::SetCursorPosition(uint32_t position)
{
    if (position == cursorPosition)
        return;

    cursorPosition = position;
    drawCursor = true;
    cursorDirty = true;

    cursorBlinkProgress = 0;
    cursorIsVisible = true;
    cursorPosition = position;
}

void UIInputField::DisableCursor()
{
    if (!drawCursor)
        return;
    drawCursor = false;
    cursorDirty = true;
    if (cursor != NULL_ENTITY)
        Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(cursor, false, true);
}

void UIInputField::UpdateCursorBlink()
{
    cursorBlinkProgress += Time::UnscaledDeltaTime();
    if (cursorBlinkProgress > cursorBlinkRate)
    {
        cursorBlinkProgress -= cursorBlinkRate;
        cursorIsVisible = !cursorIsVisible;
        cursorDirty = true;
        auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        entitiesRegistry->EntitySetActive(cursor, cursorIsVisible, true);
    }
}

void UIInputField::RebuildCursor(UIText& uiText, RectTransformation& uiTextRT)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (!cursorIsVisible || !drawCursor)
    {
        entitiesRegistry->EntitySetActive(cursor, false, true);
        return;
    }

    auto& atlas = uiText.GetFont()->characters[uiText.GetTextSize()];
    auto rectSize = uiTextRT.GetRealSizeCached();
    auto& rectMatrix = uiTextRT.GetTransformationMatrixCached();

    if (cursor == NULL_ENTITY)
    {
        // Create cursor renderer if it was not already
        auto cursorSprite = Application::Instance->GetCurrentScene()->GetUILayer()->UIResources.DefaultPixelSprite;
        cursor = entitiesRegistry->CreateNewEntity();

        auto& cursorRenderer = entitiesRegistry->AddComponent<UIQuadRenderer>(cursor);
        cursorRenderer.Color = uiText.GetColor();
        cursorRenderer.TextureID = cursorSprite->TextureID;
        cursorRenderer.Queue = RenderingQueue::Text;
        cursorRenderer.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
        cursorRenderer.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
        cursorRenderer.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
        cursorRenderer.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
    }

    float width = (float)cursorWidth / rectSize.x;
    uint32_t realPosition = std::min(uiText.GetText().size(), cursorPosition);

    bool isRendered;
    glm::vec2 origin = uiText.GetLetterOrigin(realPosition, isRendered);
    if (!isRendered)
    {
        entitiesRegistry->EntitySetActive(cursor, false, true);
        return;
    }

    // Place cursor in the origin of the letter where it is positioned
    float ox = origin.x / rectSize.x - 0.5f;
    float oy = origin.y / rectSize.y - 0.5f;

    float up = (float)atlas.MaxY / rectSize.y;
    float down = (float)atlas.MinY / rectSize.y;

    isRendered = ox >= -0.5f && ox <= 0.5f && oy + down >= -0.5f && oy + up <= 0.5f;
    if (!isRendered)
    {
        entitiesRegistry->EntitySetActive(cursor, false, true);
        return;
    }

    entitiesRegistry->EntitySetActive(cursor, true, true);
    auto& cursorRenderer = entitiesRegistry->GetComponent<UIQuadRenderer>(cursor);
    cursorRenderer.DefaultVertices[0] = glm::vec4(ox + width, oy + up, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[1] = glm::vec4(ox + width, oy + down, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[2] = glm::vec4(ox, oy + up, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[3] = glm::vec4(ox, oy + down, 0.0f, 1.0f);

    for (int j = 0; j < 4; ++j)
        cursorRenderer.Vertices[j] = rectMatrix * cursorRenderer.DefaultVertices[j];
}

void UIInputField::UpdateCursorColor(UIText& uiText) const
{
    if (cursor == NULL_ENTITY)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& cursorRenderer = entitiesRegistry->GetComponent<UIQuadRenderer>(cursor);
    cursorRenderer.Color = autoCursorColor ? uiText.GetColor() : cursorColor;
}
