#pragma once

#include "../UIComponent.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"
#include "../UIInteractable.h"
#include "UIText.h"

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

typedef std::function<void(EntityID, std::string)> InputFieldSubmitCallback;

class UIInputField : public UIInteractable
{
public:
    explicit UIInputField(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { }

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void Update();
    void Rebuild(UILayer* layer, RectTransformation& transformation);

    void SetTargetText(EntityID targetID);
    EntityID GetTargetText() const;
    void SetCursorWidth(uint32_t width);
    uint32_t GetCursorWidth() const;
    void SetCursorColor(glm::vec4 color);
    glm::vec4 GetCursorColor() const;
    void SetCursorAutoColor(bool isAuto);
    bool GetCursorAutoColor() const;
    void SetIsMultiline(bool isMultiline);
    bool GetIsMultiline() const;
    void SetTextType(TextTypes::TextType type);
    TextTypes::TextType GetTextType() const;
    void SetSelectionColor(glm::vec4 color);
    glm::vec4 GetSelectionColor() const;

    static bool IsInt(const std::string& text, int& value);
    static bool IsFloat(const std::string& text, float& value);
    static int IsNotAlNum(char c);

    InputFieldSubmitCallback SubmitCallback = nullptr;

private:
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
    void RebuildCursor(UIText& uiText, RectTransformation& uiTextRT, float dz);
    void UpdateCursorColor(UIText& uiText) const;
    void UpdateCursorSortingOrder(RectTransformation& uiTextRT, float dz) const;

    void SetSelection(uint32_t from, uint32_t to);
    void DisableSelection();
    void CleanSelection();
    void TryKeepSelection();
    void RemoveSelectedText(UIText& uiText);
    void RebuildSelection(UIText& uiText, RectTransformation& uiTextRT, float dz);
    void UpdateSelectionSortingOrder(RectTransformation& uiTextRT, float dz) const;
    EntityID CreateSelectionBlock(UIText& uiText, RectTransformation& uiTextRT, uint32_t from, uint32_t to, float dz);

    EntityID _targetText = NULL_ENTITY;
    bool wasEdited = false;
    uint32_t cursorPosition = -1;
    float cursorHorizontalOffset = -1;

    bool multiline = false;
    TextTypes::TextType textType = TextTypes::Standard;

    bool drawCursor = false;
    EntityID cursor = NULL_ENTITY;
    uint32_t cursorWidth = 2;
    float cursorBlinkRate = 0.53f;
    float cursorBlinkProgress = 0.0f;
    bool cursorIsVisible = true;
    glm::vec4 cursorColor = glm::vec4(0.0f);
    bool autoCursorColor = true;

    bool cursorColorDirty = false;
    bool cursorDirty = false;

    bool drawSelection = false;
    bool selectionDirty = false;
    bool isFirstSelection = false;
    uint32_t selectionStart = 0;
    uint32_t selectionEnd = 0;
    std::vector<EntityID> selectionEntites;
    glm::vec4 selectionColor = glm::vec4(0.42f, 0.57f, 0.7f, 1.0f);

    friend class UISystem;
};
