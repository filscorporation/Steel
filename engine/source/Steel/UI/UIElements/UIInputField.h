#pragma once

#include "UIText.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/UIEvent.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/UI/UIInteractable.h"

namespace TextTypes
{
    enum TextType
    {
        Standard,
        IntegerNumber,
        DecimalNumber,
        Alphanumeric,
    };
}

typedef std::function<void(EntityID, std::string)> InputFieldChangedCallback;
typedef std::function<void(EntityID, std::string)> InputFieldSubmitCallback;

class UIInputField : public UIInteractable
{
    DEFINE_COMPONENT(UIInputField)

public:
    explicit UIInputField(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { }

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void Update();
    void Rebuild(RectTransformation& transformation);
    void Draw(RenderContext* renderContext);

    void SetTargetText(EntityID targetID);
    EntityID GetTargetText() const;
    void SetCursorWidth(uint32_t width);
    uint32_t GetCursorWidth() const;
    void SetCursorColor(const glm::vec4& color);
    const glm::vec4& GetCursorColor() const;
    void SetCursorAutoColor(bool isAuto);
    bool GetCursorAutoColor() const;
    void SetIsMultiline(bool isMultiline);
    bool GetIsMultiline() const;
    void SetTextType(TextTypes::TextType type);
    TextTypes::TextType GetTextType() const;
    void SetSelectionColor(const glm::vec4& color);
    const glm::vec4& GetSelectionColor() const;

    static bool IsInt(const std::string& text, int& value);
    static bool IsFloat(const std::string& text, float& value);
    static int IsNotAlNum(char c);

    InputFieldChangedCallback OnChangedCallback = nullptr;
    InputFieldSubmitCallback SubmitCallback = nullptr;

private:
    void RebuildCursorInner(RectTransformation& transformation);
    void RebuildSelectionInner(RectTransformation& transformation);

    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);
    void Select(UIText& uiText);
    void Disselect(UIText& uiText);
    void AddText(UIText& uiText, const std::string& text);
    int SetText(UIText& uiText, std::string& text);
    void Validate(const std::string& oldText, std::string& newText, bool submit);

    void SetCursorPosition(uint32_t position);
    void DisableCursor();
    void UpdateCursorBlink();

    void SetSelection(uint32_t from, uint32_t to);
    void DisableSelection();
    void TryKeepSelection();
    void RemoveSelectedText(UIText& uiText);

    bool initialized = false;
    EntityID _targetText = NULL_ENTITY;
    bool wasEdited = false;
    uint32_t cursorPosition = -1;
    float cursorHorizontalOffset = -1;

    bool multiline = false;
    TextTypes::TextType textType = TextTypes::Standard;

    bool drawCursor = false;
    uint32_t cursorWidth = 2;
    float cursorBlinkRate = 0.53f;
    float cursorBlinkProgress = 0.0f;
    bool cursorIsVisible = true;
    glm::vec4 cursorColor = glm::vec4(0.0f);
    bool autoCursorColor = true;

    bool drawSelection = false;
    bool isFirstSelection = false;
    uint32_t selectionStart = 0;
    uint32_t selectionEnd = 0;
    glm::vec4 selectionColor = glm::vec4(0.42f, 0.57f, 0.7f, 1.0f);

    bool isCursorDirty = false;
    VertexBuffer vbCursor;
    IndexBuffer ibCursor;
    float sortingOrderCursor = 0;
    Material* materialCursor = nullptr;
    MaterialPropertyBlock propertiesCursor;

    bool isSelectionDirty = false;
    VertexBuffer vbSelection;
    IndexBuffer ibSelection;
    float sortingOrderSelection = 0;
    Material* materialSelection = nullptr;
    MaterialPropertyBlock propertiesSelection;
};
