#include "UIInputField.h"
#include "Steel/Core/Time.h"
#include "Steel/Input/Input.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scripting/ScriptingCore.h"
#include "Steel/Scripting/ScriptingSystem.h"

#include <algorithm>
#include <string>
#include <sstream>

void UIInputField::RegisterType()
{
    REGISTER_COMPONENT(UIInputField);
    REGISTER_ID_ATTRIBUTE(UIInputField, "targetText", GetTargetText, SetTargetText, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "cursorWidth", GetCursorWidth, SetCursorWidth, uint32_t, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "cursorColor", GetCursorColor, SetCursorColor, glm::vec4, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "cursorAutoColor", GetCursorAutoColor, SetCursorAutoColor, bool, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "isMultiline", GetIsMultiline, SetIsMultiline, bool, AttributeFlags::Public);
    REGISTER_ENUM_ATTRIBUTE(UIInputField, "textType", GetTextType, SetTextType, TextTypes::TextType, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "selectionColor", GetSelectionColor, SetSelectionColor, glm::vec4, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "transitionInfo", GetTransitionsInfo, SetTransitionsInfo, TransitionsInfo, AttributeFlags::Public);
    REGISTER_ID_ATTRIBUTE(UIInputField, "targetImage", GetTargetImage, SetTargetImage, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(UIInputField, "isInteractable", GetInteractable, SetInteractable, bool, AttributeFlags::Public);
}

bool UIInputField::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIInputField::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    UIInteractable::SetDefaultTransitionInfo();
}

void UIInputField::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    initialized = true;
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UIInputField::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::TextInput | UIEventTypes::KeyInput |
                              UIEventTypes::MouseEnter | UIEventTypes::MouseExit | UIEventTypes::MouseJustPressed |
                              UIEventTypes::MouseDragBegin | UIEventTypes::MouseDrag | UIEventTypes::MouseDragEnd |
                              UIEventTypes::MouseJustPressedAnywhere;

    UIInteractable::InitTransitionInfo(UpdateTransition);
}

void UIInputField::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    vbCursor.Clear();
    ibCursor.Clear();
    vbSelection.Clear();
    ibSelection.Clear();

    StopTransition();
    ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::InputFieldChangeValue, ScriptingCore::GetDeregisterCallbacksMethod());
    ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::InputFieldEndEdit, ScriptingCore::GetDeregisterCallbacksMethod());
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

void UIInputField::Rebuild(RectTransformation& transformation)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_targetText == NULL_ENTITY || !entitiesRegistry->EntityExists(_targetText))
    {
        DisableCursor();
        DisableSelection();
        return;
    }

    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    auto& uiTextRT = entitiesRegistry->GetComponent<RectTransformation>(_targetText);

    if (drawCursor && uiText.IsDirty())
    {
        isCursorDirty = true;
        isSelectionDirty = true;
        SetCursorPosition(std::min((uint32_t)uiText.GetText().size(), cursorPosition));
    }

    if (isCursorDirty)
        RebuildCursorInner(transformation);
    if (isSelectionDirty)
        RebuildSelectionInner(transformation);
}

void UIInputField::Draw(RenderContext* renderContext)
{
    if (isCursorDirty)
        RebuildCursorInner(GetComponentS<RectTransformation>(Owner));
    if (isSelectionDirty)
        RebuildSelectionInner(GetComponentS<RectTransformation>(Owner));

    if (cursorIsVisible && !vbCursor.IsEmpty() && !ibCursor.IsEmpty())
    {
        DrawCall drawCall;
        drawCall.VB = vbCursor;
        drawCall.IB = ibCursor;
        drawCall.RenderMaterial = materialCursor;
        drawCall.CustomProperties = propertiesCursor;
        drawCall.SortingOrder = sortingOrderCursor;
        drawCall.Queue = RenderingQueue::Opaque;

        renderContext->List.AddDrawCall(drawCall);
    }

    if (!vbSelection.IsEmpty() && !ibSelection.IsEmpty())
    {
        DrawCall drawCall;
        drawCall.VB = vbSelection;
        drawCall.IB = ibSelection;
        drawCall.RenderMaterial = materialSelection;
        drawCall.CustomProperties = propertiesSelection;
        drawCall.SortingOrder = sortingOrderSelection;
        drawCall.Queue = RenderingQueue::Opaque;

        renderContext->List.AddDrawCall(drawCall);
    }
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

void UIInputField::SetCursorColor(const glm::vec4& color)
{
    cursorColor = color;
    autoCursorColor = false;
    isCursorDirty = true;
}

const glm::vec4& UIInputField::GetCursorColor() const
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

    if (!initialized)
        return;

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

    if (!initialized)
        return;

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

void UIInputField::SetSelectionColor(const glm::vec4& color)
{
    selectionColor = color;
    isSelectionDirty = true;
}

const glm::vec4& UIInputField::GetSelectionColor() const
{
    return selectionColor;
}

void UIInputField::RebuildCursorInner(RectTransformation& transformation)
{
    isCursorDirty = false;

    ibCursor.Clear();
    vbCursor.Clear();

    if (!drawCursor)
        return;

    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();
    if (materialCursor == nullptr)
    {
        materialCursor = Application::Instance->GetResourcesManager()->DefaultUIMaterial();
        auto cursorSprite = layer->UIResources.DefaultPixelSprite;
        propertiesCursor.SetTexture(MAIN_TEX, cursorSprite->SpriteTexture->GetTextureID());
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    auto& uiTextRT = entitiesRegistry->GetComponent<RectTransformation>(_targetText);
    auto& atlas = uiText.GetFont()->characters[uiText.GetTextSize()];
    auto rectSize = uiTextRT.GetRealSizeCached();
    auto& matrix = uiTextRT.GetTransformationMatrixCached();

    if (autoCursorColor)
        cursorColor = uiText.GetColor();

    float width = (float)cursorWidth / rectSize.x;
    uint32_t realPosition = std::min((uint32_t)uiText.GetText().size(), cursorPosition);

    bool isRendered;
    glm::vec2 origin = uiText.GetLetterOrigin(realPosition, isRendered);
    if (!isRendered)
        return;

    // Place cursor in the origin of the letter where it is positioned
    float ox = origin.x / rectSize.x - 0.5f;
    float oy = origin.y / rectSize.y - 0.5f;

    float up = (float)atlas.MaxY / rectSize.y;
    float down = (float)atlas.MinY / rectSize.y;

    isRendered = ox >= -0.5f && ox <= 0.5f && oy + down >= -0.5f && oy + up <= 0.5f;
    if (!isRendered)
        return;

    float dz = 1.0f / (float)layer->GetLayerThickness();
    sortingOrderCursor = uiTextRT.GetSortingOrder() + dz * 0.1f;

    glm::vec2 texCoords[4];
    texCoords[0] = glm::vec2(1.0f, 0.0f);
    texCoords[1] = glm::vec2(1.0f, 1.0f);
    texCoords[2] = glm::vec2(0.0f, 0.0f);
    texCoords[3] = glm::vec2(0.0f, 1.0f);

    glm::vec3 vertices[4];
    vertices[0] = matrix * glm::vec4(ox + width, oy + up, dz * 0.1f, 1.0f);
    vertices[1] = matrix * glm::vec4(ox + width, oy + down, dz * 0.1f, 1.0f);
    vertices[2] = matrix * glm::vec4(ox, oy + up, dz * 0.1f, 1.0f);
    vertices[3] = matrix * glm::vec4(ox, oy + down, dz * 0.1f, 1.0f);

    auto indices = new uint32_t[6]{ 0, 1, 2, 1, 2, 3 };

    ibCursor.Create(indices, 6);
    vbCursor.Create(vertices, cursorColor, texCoords);
}

void UIInputField::RebuildSelectionInner(RectTransformation& transformation)
{
    isSelectionDirty = false;

    ibSelection.Clear();
    vbSelection.Clear();

    if (!drawSelection || selectionStart == selectionEnd)
        return;

    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();
    if (materialSelection == nullptr)
    {
        materialSelection = Application::Instance->GetResourcesManager()->DefaultUIMaterial();
        auto selectionSprite = layer->UIResources.DefaultPixelSprite;
        propertiesSelection.SetTexture(MAIN_TEX, selectionSprite->SpriteTexture->GetTextureID());
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& uiText = entitiesRegistry->GetComponent<UIText>(_targetText);
    auto& uiTextRT = entitiesRegistry->GetComponent<RectTransformation>(_targetText);
    auto& atlas = uiText.GetFont()->characters[uiText.GetTextSize()];
    auto rectSize = uiTextRT.GetRealSizeCached();
    auto& matrix = uiTextRT.GetTransformationMatrixCached();

    float dz = 1.0f / (float)layer->GetLayerThickness();
    sortingOrderSelection = uiTextRT.GetSortingOrder() - dz * 0.1f;

    std::vector<std::tuple<uint32_t, uint32_t>> linesIndices;
    uiText.GetLinesIndices(std::min(selectionStart, selectionEnd), std::max(selectionStart, selectionEnd), linesIndices);

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    uint32_t linesRendered = 0;
    for (auto& lineIndexPair : linesIndices)
    {
        uint32_t from = std::get<0>(lineIndexPair);
        uint32_t to = std::get<1>(lineIndexPair);

        bool isRenderedFrom, isRenderedTo;
        glm::vec3 originFrom = uiText.GetLetterOrigin(from, isRenderedFrom);
        glm::vec3 originTo = uiText.GetLetterOrigin(to, isRenderedTo);
        if (!isRenderedFrom || !isRenderedTo)
            continue;

        float ox1 = originFrom.x / rectSize.x - 0.5f;
        float oy1 = originFrom.y / rectSize.y - 0.5f;
        float ox2 = originTo.x / rectSize.x - 0.5f;
        float oy2 = originTo.y / rectSize.y - 0.5f;

        float up = (float)atlas.MaxY / rectSize.y;
        float down = (float)atlas.MinY / rectSize.y;

        bool isRendered = (ox1 >= -0.5f || ox2 >= -0.5f) && (ox1 <= 0.5f || ox2 <= 0.5f)
                          && (oy1 + down >= -0.5f || oy2 + down >= -0.5f) && (oy1 + up <= 0.5f || oy2 + up <= 0.5f);

        if (!isRendered)
            continue;

        indices.reserve(linesRendered * 6);
        indices.push_back(linesRendered * 4 + 0);
        indices.push_back(linesRendered * 4 + 1);
        indices.push_back(linesRendered * 4 + 2);
        indices.push_back(linesRendered * 4 + 1);
        indices.push_back(linesRendered * 4 + 2);
        indices.push_back(linesRendered * 4 + 3);

        glm::vec2 texCoords[4];
        texCoords[0] = glm::vec2(1.0f, 0.0f);
        texCoords[1] = glm::vec2(1.0f, 1.0f);
        texCoords[2] = glm::vec2(0.0f, 0.0f);
        texCoords[3] = glm::vec2(0.0f, 1.0f);

        ox1 = std::min(0.5f, std::max(-0.5f, ox1));
        ox2 = std::min(0.5f, std::max(-0.5f, ox2));
        oy1 = std::min(0.5f, std::max(-0.5f, oy1));
        oy2 = std::min(0.5f, std::max(-0.5f, oy2));

        glm::vec3 vectorVertices[4];
        vectorVertices[0] = matrix * glm::vec4(ox2, oy2 + up, -dz * 0.1f, 1.0f);
        vectorVertices[1] = matrix * glm::vec4(ox2, oy2 + down, -dz * 0.1f, 1.0f);
        vectorVertices[2] = matrix * glm::vec4(ox1, oy1 + up, -dz * 0.1f, 1.0f);
        vectorVertices[3] = matrix * glm::vec4(ox1, oy1 + down, -dz * 0.1f, 1.0f);

        vertices.reserve(linesRendered * 9 * 4);
        for (int i = 0; i < 4; ++i)
        {
            vertices.push_back(vectorVertices[i][0]);
            vertices.push_back(vectorVertices[i][1]);
            vertices.push_back(vectorVertices[i][2]);
            vertices.push_back(selectionColor[0]);
            vertices.push_back(selectionColor[1]);
            vertices.push_back(selectionColor[2]);
            vertices.push_back(selectionColor[3]);
            vertices.push_back(texCoords[i][0]);
            vertices.push_back(texCoords[i][1]);
        }

        linesRendered++;
    }

    if (linesRendered == 0)
        return;

    std::vector<VertexAttribute> attributes;
    attributes.reserve(3);
    attributes.emplace_back(0, 3);
    attributes.emplace_back(1, 4);
    attributes.emplace_back(2, 2);

    auto indicesData = new uint32_t[indices.size()];
    std::copy(indices.begin(), indices.end(), indicesData);
    auto verticesData = new float[vertices.size()];
    std::copy(vertices.begin(), vertices.end(), verticesData);

    ibSelection.Create(indicesData, indices.size());
    vbSelection.Create(verticesData, vertices.size(), attributes);
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
                auto text = uiText.GetText();
                int diff = SetText(uiText, text.erase(cursorPosition - 1, 1));
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
                auto text = uiText.GetText();
                SetText(uiText, text.erase(cursorPosition, 1));
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
            ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::InputFieldEndEdit,
                                              ScriptingCore::GetInvokeCallbacksMethod());
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
    int diffFromSelected = 0;
    if (selectionStart != selectionEnd)
    {
        // Remove selected text
        uint32_t from = std::min(selectionStart, selectionEnd);
        uint32_t len = std::abs((int)selectionStart - (int)selectionEnd);
        diffFromSelected = len;
        newText = uiText.GetText();
        newText = newText.erase(from, len);
        SetCursorPosition(from);
        cursorHorizontalOffset = -1;
    }
    else
        newText = uiText.GetText();

    uint32_t offset = std::min(cursorPosition, (uint32_t)newText.size());
    newText.insert(offset, text);

    int diff = SetText(uiText, newText);
    diff += diffFromSelected; // We do not want to move cursor from changes of removed selected text, only the one we added
    SetCursorPosition(cursorPosition + diff);
    selectionEnd = cursorPosition;
    selectionStart = cursorPosition;
    cursorHorizontalOffset = -1;
}

int UIInputField::SetText(UIText& uiText, std::string& text)
{
    Validate(uiText.GetText(), text, false);
    int diff = (int)text.size() - (int)(uiText.GetText().size());
    uiText.SetText(text);
    wasEdited = true;

    if (ScriptingSystem::IsInitialized())
        ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::InputFieldChangeValue,
                                          ScriptingCore::GetInvokeCallbacksMethod());

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
    if (!drawCursor)
        return;

    drawCursor = false;
    vbCursor.Clear();
    ibCursor.Clear();
}

void UIInputField::UpdateCursorBlink()
{
    cursorBlinkProgress += Time::UnscaledDeltaTime();
    if (cursorBlinkProgress > cursorBlinkRate)
    {
        cursorBlinkProgress -= cursorBlinkRate;
        cursorIsVisible = !cursorIsVisible;
    }
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
    vbSelection.Clear();
    ibSelection.Clear();
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
        selectionEnd = cursorPosition;
    }
}

void UIInputField::RemoveSelectedText(UIText& uiText)
{
    if (selectionStart == selectionEnd)
        return;

    uint32_t from = std::min(selectionStart, selectionEnd);
    uint32_t len = std::abs((int)selectionStart - (int)selectionEnd);
    auto text = uiText.GetText();
    SetText(uiText, text.erase(from, len));
    SetCursorPosition(from);
    selectionStart = from;
    selectionEnd = from;
    cursorHorizontalOffset = -1;
}
