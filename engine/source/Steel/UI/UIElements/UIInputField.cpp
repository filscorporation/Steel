#include <algorithm>
#include <string>
#include <sstream>

#include "UIInputField.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Time.h"
#include "Steel/Input/Input.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scripting/ScriptingCore.h"
#include "Steel/Scripting/ScriptingSystem.h"

bool UIInputField::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIInputField::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UIInputField::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::TextInput | UIEventTypes::KeyInput |
                              UIEventTypes::MouseEnter | UIEventTypes::MouseExit | UIEventTypes::MouseJustPressed |
                              UIEventTypes::MouseDragBegin | UIEventTypes::MouseDrag | UIEventTypes::MouseDragEnd |
                              UIEventTypes::MouseJustPressedAnywhere;

    UIInteractable::Init(UpdateTransition);
}

void UIInputField::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    /*if (cursor != NULL_ENTITY)
        entitiesRegistry->DeleteEntity(cursor);
    cursor = NULL_ENTITY;
    for (auto& selectionEntity : selectionEntites)
        entitiesRegistry->DeleteEntity(selectionEntity);
    selectionEntites.clear();*/

    StopTransition();
    ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldChangeValue, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
    ScriptingCore::CallEventMethod(Owner, CallbackTypes::InputFieldEndEdit, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
}

void UIInputField::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isSelectionDirty = true;
    isCursorDirty = true;

    RestoreTransition();
}

void UIInputField::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
        return;
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    Disselect(uiText);
}

void UIInputField::Update()
{
    if (drawCursor)
        UpdateCursorBlink();
}

void UIInputField::Rebuild(UILayer* layer, RectTransformation& transformation)
{
    /*
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
    {
        if (drawCursor)
        {
            entitiesRegistry->EntitySetActive(cursor, false, true);
            drawCursor = false;
        }
        if (drawSelection)
        {
            CleanSelection();
            drawSelection = false;
        }
        return;
    }

    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    auto& uiTextRT = entitiesRegistry->GetComponent<RectTransformation>(_targetText);
    if (uiText.IsTextColorDirty() || cursorColorDirty)
    {
        UpdateCursorColor(uiText);
    }
    if (drawCursor && uiText.IsTextDirty())
    {
        SetCursorPosition(std::min((uint32_t)uiText.GetText().size(), cursorPosition));
    }
    bool otherDirty = uiTextRT.DidTransformationChange() || uiText.IsTextDirty() || transformation.DidTransformationChange();
    float dz = 1.0f / (float)layer->GetLayerThickness();
    bool cursorRebuilt = false, selectionRebuilt = false;
    if (otherDirty || cursorDirty)
    {
        cursorRebuilt = true;
        RebuildCursor(uiText, uiTextRT, dz);
    }
    if (otherDirty || selectionDirty)
    {
        selectionRebuilt = true;
        RebuildSelection(uiText, uiTextRT, dz);
    }
    if (layer->NeedRebuildSortingOrder())
    {
        if (!cursorRebuilt)
            UpdateCursorSortingOrder(uiTextRT, dz);
        if (!selectionRebuilt)
            UpdateSelectionSortingOrder(uiTextRT, dz);
    }

    cursorDirty = false;
    cursorColorDirty = false;
    selectionDirty = false;*/
}

void UIInputField::Draw(RenderContext* renderContext)
{
    if (isCursorDirty)
        RebuildCursorInner(GetComponentS<RectTransformation>(Owner));
    if (isSelectionDirty)
        RebuildSelectionInner(GetComponentS<RectTransformation>(Owner));
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
    isCursorDirty = true;
}

uint32_t UIInputField::GetCursorWidth() const
{
    return cursorWidth;
}

void UIInputField::SetCursorColor(glm::vec4 color)
{
    cursorColor = color;
    autoCursorColor = false;
    isCursorDirty = true; // TODO: ?
}

glm::vec4 UIInputField::GetCursorColor() const
{
    return cursorColor;
}

void UIInputField::SetCursorAutoColor(bool isAuto)
{
    autoCursorColor = isAuto;
    isCursorDirty = true;
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

void UIInputField::SetSelectionColor(glm::vec4 color)
{
    selectionColor = color;
    isSelectionDirty = true;
}

glm::vec4 UIInputField::GetSelectionColor() const
{
    return selectionColor;
}

void UIInputField::RebuildCursorInner(RectTransformation& transformation)
{
    isCursorDirty = false;

    // TODO:
}

void UIInputField::RebuildSelectionInner(RectTransformation& transformation)
{
    isSelectionDirty = false;

    // TODO:
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
            PlayTransition(TransitionStates::Hovered);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        IsHovered = false;
        if (IsSelected)
            PlayTransition(TransitionStates::Selected);
        else
            PlayTransition(TransitionStates::Normal);
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
        return;
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);

    if (eventType & UIEventTypes::MouseDragBegin)
    {
        if (!IsSelected)
        {
            isFirstSelection = true;
            Select(uiText);
        }
        uint32_t newPosition = uiText.GetLetterPosition(uiEvent.MousePosition);
        cursorHorizontalOffset = -1;
        SetCursorPosition(newPosition);
        selectionStart = newPosition;
    }
    if (eventType & UIEventTypes::MouseDragEnd)
    {
        if (isFirstSelection)
        {
            if (selectionStart == selectionEnd)
            {
                // Select full text when field selected without a drag
                SetSelection(0, uiText.GetText().size());
            }
            isFirstSelection = false;
        }
    }
    if (eventType & UIEventTypes::MouseDrag)
    {
        if (IsSelected)
        {
            uint32_t newPosition = uiText.GetLetterPosition(uiEvent.MousePosition);
            cursorHorizontalOffset = -1;
            SetCursorPosition(newPosition);
            SetSelection(selectionStart, newPosition);
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
            if (selectionStart != selectionEnd)
            {
                RemoveSelectedText(uiText);
            }
            else if (cursorPosition > 0 && !uiText.GetText().empty())
            {
                int diff = SetText(uiText, uiText.GetText().erase(cursorPosition - 1, 1));
                SetCursorPosition(cursorPosition + diff);
                cursorHorizontalOffset = -1;
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Delete))
        {
            if (selectionStart != selectionEnd)
            {
                RemoveSelectedText(uiText);
            }
            else if (cursorPosition < uiText.GetText().size() && !uiText.GetText().empty())
            {
                SetText(uiText, uiText.GetText().erase(cursorPosition, 1));
            }
        }
        if (Input::IsKeyJustPressed(KeyCodes::Enter))
        {
            if (multiline)
            {
                AddText(uiText, "\n");
            }
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
                SetCursorPosition(std::min((uint32_t)uiText.GetText().size(), cursorPosition) - 1);
                cursorHorizontalOffset = -1;
            }
            TryKeepSelection();
        }
        if (Input::IsKeyJustPressed(KeyCodes::Right))
        {
            if (cursorPosition < uiText.GetText().size())
            {
                SetCursorPosition(cursorPosition + 1);
                cursorHorizontalOffset = -1;
            }
            TryKeepSelection();
        }
        if (Input::IsKeyJustPressed(KeyCodes::Up))
        {
            uint32_t newPosition = uiText.GetLetterPositionLineUp(cursorPosition, cursorHorizontalOffset);
            SetCursorPosition(newPosition);
            TryKeepSelection();
        }
        if (Input::IsKeyJustPressed(KeyCodes::Down))
        {
            uint32_t newPosition = uiText.GetLetterPositionLineDown(cursorPosition, cursorHorizontalOffset);
            SetCursorPosition(newPosition);
            TryKeepSelection();
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
    PlayTransition(TransitionStates::Selected);
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
    isFirstSelection = false;
    DisableCursor();
    DisableSelection();
    if (IsInteractable)
    {
        if (IsHovered)
            PlayTransition(TransitionStates::Hovered);
        else
            PlayTransition(TransitionStates::Normal);
    }
}

void UIInputField::AddText(UIText& uiText, const std::string& text)
{
    std::string newText;
    if (selectionStart != selectionEnd)
    {
        // Remove selected text
        uint32_t from = std::min(selectionStart, selectionEnd);
        uint32_t len = std::abs((int)selectionStart - (int)selectionEnd);
        newText = uiText.GetText().erase(from, len);
        SetCursorPosition(from);
        selectionEnd = from;
        selectionStart = from;
        cursorHorizontalOffset = -1;
    }
    else
        newText = uiText.GetText();

    uint32_t offset = std::min(cursorPosition, (uint32_t)newText.size());
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

bool UIInputField::IsInt(const std::string& text, int& value)
{
    std::istringstream iss(text);
    iss >> std::noskipws >> value;
    return iss.eof() && !iss.fail();
}

bool UIInputField::IsFloat(const std::string& text, float& value)
{
    std::istringstream iss(text);
    iss >> std::noskipws >> value;
    return iss.eof() && !iss.fail();
}

int UIInputField::IsNotAlNum(char c)
{
    return !std::isalnum(c);
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
            newText.erase(std::remove_if(newText.begin(), newText.end(), IsNotAlNum), newText.end());
            break;
    }
}

void UIInputField::SetCursorPosition(uint32_t position)
{
    if (position == cursorPosition)
        return;

    cursorPosition = position;
    drawCursor = true;
    isCursorDirty = true;

    cursorBlinkProgress = 0;
    cursorIsVisible = true;
    cursorPosition = position;
}

void UIInputField::DisableCursor()
{
    /*
    if (!drawCursor)
        return;
    drawCursor = false;
    cursorDirty = true;
    if (cursor != NULL_ENTITY)
        Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(cursor, false, true);*/
}

void UIInputField::UpdateCursorBlink()
{
    /*
    cursorBlinkProgress += Time::UnscaledDeltaTime();
    if (cursorBlinkProgress > cursorBlinkRate)
    {
        cursorBlinkProgress -= cursorBlinkRate;
        cursorIsVisible = !cursorIsVisible;
        isCursorDirty = true;
        auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        entitiesRegistry->EntitySetActive(cursor, cursorIsVisible, true);
    }*/
}

void UIInputField::RebuildCursor(UIText& uiText, RectTransformation& uiTextRT, float dz)
{
    /*auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
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
        cursorRenderer.RenderMaterial = Application::Instance->GetResourcesManager()->DefaultUIMaterial();
        cursorRenderer.CustomProperties.SetTexture(MAIN_TEX, cursorSprite->SpriteTexture->GetTextureID());
        cursorRenderer.Queue = RenderingQueue::Opaque;
        cursorRenderer.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
        cursorRenderer.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
        cursorRenderer.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
        cursorRenderer.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
    }

    float width = (float)cursorWidth / rectSize.x;
    uint32_t realPosition = std::min((uint32_t)uiText.GetText().size(), cursorPosition);

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
    cursorRenderer.DefaultVertices[0] = glm::vec4(ox + width, oy + up, dz * 0.1f, 1.0f);
    cursorRenderer.DefaultVertices[1] = glm::vec4(ox + width, oy + down, dz * 0.1f, 1.0f);
    cursorRenderer.DefaultVertices[2] = glm::vec4(ox, oy + up, dz * 0.1f, 1.0f);
    cursorRenderer.DefaultVertices[3] = glm::vec4(ox, oy + down, dz * 0.1f, 1.0f);
    cursorRenderer.SortingOrder = uiTextRT.GetSortingOrder() + dz * 0.1f;

    for (int j = 0; j < 4; ++j)
        cursorRenderer.Vertices[j] = rectMatrix * cursorRenderer.DefaultVertices[j];*/
}

void UIInputField::SetSelection(uint32_t from, uint32_t to)
{
    if (drawSelection && from == selectionStart && to == selectionEnd)
        return;

    drawSelection = true;
    isSelectionDirty = true;
    selectionStart = from;
    selectionEnd = to;
}

void UIInputField::DisableSelection()
{
    if (!drawSelection)
        return;

    drawSelection = false;
    CleanSelection();
}

void UIInputField::CleanSelection()
{
    /*
    if (selectionEntites.empty())
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    for (auto& selectionEntity : selectionEntites)
        entitiesRegistry->DeleteEntity(selectionEntity);
    selectionEntites.clear();*/
}

void UIInputField::TryKeepSelection()
{
    bool keepSelection = Input::IsKeyPressed(KeyCodes::LeftShift) || Input::IsKeyPressed(KeyCodes::RightShift);

    if (keepSelection)
    {
        SetSelection(selectionStart, cursorPosition);
    }
    else
    {
        DisableSelection();
        selectionStart = cursorPosition;
    }
}

void UIInputField::RemoveSelectedText(UIText& uiText)
{
    if (selectionStart == selectionEnd)
        return;

    uint32_t from = std::min(selectionStart, selectionEnd);
    uint32_t len = std::abs((int)selectionStart - (int)selectionEnd);
    SetText(uiText, uiText.GetText().erase(from, len));
    SetCursorPosition(from);
    selectionEnd = from;
    selectionStart = from;
    cursorHorizontalOffset = -1;
}

void UIInputField::RebuildSelection(UIText& uiText, RectTransformation& uiTextRT, float dz)
{
    /*if (!drawSelection)
        return;

    CleanSelection();

    if (selectionStart == selectionEnd)
        return;

    std::vector<std::tuple<uint32_t, uint32_t>> indices;
    uiText.GetLinesIndices(std::min(selectionStart, selectionEnd), std::max(selectionStart, selectionEnd), indices);

    for (auto& indexPair : indices)
    {
        selectionEntites.push_back(CreateSelectionBlock(uiText, uiTextRT, std::get<0>(indexPair), std::get<1>(indexPair), dz));
    }*/
}

EntityID UIInputField::CreateSelectionBlock(UIText& uiText, RectTransformation& uiTextRT, uint32_t from, uint32_t to, float dz)
{
    /*
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    auto& atlas = uiText.GetFont()->characters[uiText.GetTextSize()];
    auto rectSize = uiTextRT.GetRealSizeCached();
    auto& rectMatrix = uiTextRT.GetTransformationMatrixCached();

    auto pixelSprite = Application::Instance->GetCurrentScene()->GetUILayer()->UIResources.DefaultPixelSprite;

    bool isRenderedFrom, isRenderedTo;
    glm::vec3 originFrom = uiText.GetLetterOrigin(from, isRenderedFrom);
    glm::vec3 originTo = uiText.GetLetterOrigin(to, isRenderedTo);
    if (!isRenderedFrom || !isRenderedTo)
        return NULL_ENTITY;

    float ox1 = originFrom.x / rectSize.x - 0.5f;
    float oy1 = originFrom.y / rectSize.y - 0.5f;
    float ox2 = originTo.x / rectSize.x - 0.5f;
    float oy2 = originTo.y / rectSize.y - 0.5f;

    float up = (float)atlas.MaxY / rectSize.y;
    float down = (float)atlas.MinY / rectSize.y;

    bool isRendered = (ox1 >= -0.5f || ox2 >= -0.5f) && (ox1 <= 0.5f || ox2 <= 0.5f)
            && (oy1 + down >= -0.5f || oy2 + down >= -0.5f) && (oy1 + up <= 0.5f || oy2 + up <= 0.5f);

    if (!isRendered)
        return NULL_ENTITY;

    ox1 = std::min(0.5f, std::max(-0.5f, ox1));
    ox2 = std::min(0.5f, std::max(-0.5f, ox2));
    oy1 = std::min(0.5f, std::max(-0.5f, oy1));
    oy2 = std::min(0.5f, std::max(-0.5f, oy2));

    EntityID entity = entitiesRegistry->CreateNewEntity();

    auto& blockRenderer = entitiesRegistry->AddComponent<UIQuadRenderer>(entity);
    blockRenderer.Color = selectionColor;
    blockRenderer.RenderMaterial = Application::Instance->GetResourcesManager()->DefaultUIMaterial();
    blockRenderer.CustomProperties.SetTexture(MAIN_TEX, pixelSprite->SpriteTexture->GetTextureID());
    blockRenderer.Queue = RenderingQueue::Opaque;
    blockRenderer.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
    blockRenderer.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
    blockRenderer.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
    blockRenderer.TextureCoords[3] = glm::vec2(0.0f, 1.0f);

    blockRenderer.DefaultVertices[0] = glm::vec4(ox2, oy2 + up, -dz * 0.1f, 1.0f);
    blockRenderer.DefaultVertices[1] = glm::vec4(ox2, oy2 + down, -dz * 0.1f, 1.0f);
    blockRenderer.DefaultVertices[2] = glm::vec4(ox1, oy1 + up, -dz * 0.1f, 1.0f);
    blockRenderer.DefaultVertices[3] = glm::vec4(ox1, oy1 + down, -dz * 0.1f, 1.0f);
    blockRenderer.SortingOrder = uiTextRT.GetSortingOrder() - dz * 0.1f;

    for (int j = 0; j < 4; ++j)
        blockRenderer.Vertices[j] = rectMatrix * blockRenderer.DefaultVertices[j];

    return entity;*/
}
