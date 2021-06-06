#pragma once

#include "../UIComponent.h"
#include "../UIEvent.h"
#include "../UIEventHandler.h"
#include "../UIInteractable.h"
#include "UIText.h"

typedef std::function<void(EntityID, std::string)> InputFieldSubmitCallback;

class UIInputField : public UIInteractable
{
public:
    explicit UIInputField(EntityID ownerEntityID) : UIInteractable(ownerEntityID) { }

    void Init(UIEventHandler& eventHandler);
    void Rebuild(RectTransformation& transformation);

    void SetTargetText(EntityID targetID);
    EntityID GetTargetText() const;
    void SetCursorWidth(uint32_t width);
    uint32_t GetCursorWidth() const;
    void SetCursorColor(glm::vec4 color);
    glm::vec4 GetCursorColor() const;
    void SetCursorAutoColor(bool isAuto);
    bool GetCursorAutoColor() const;

    InputFieldSubmitCallback SubmitCallback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    static bool UpdateTransition(EntityID entityID);
    void Select(UIText& uiText);
    void Disselect(UIText& uiText);
    void AddText(UIText& uiText, const std::string& text);

    void SetCursorPosition(uint32_t position);
    void DisableCursor();
    void UpdateCursorBlink();
    void RebuildCursor(UIText& uiText, RectTransformation& uiTextRT);
    void UpdateCursorColor(UIText& uiText) const;

    EntityID _targetText = NULL_ENTITY;
    bool wasEdited = false;
    uint32_t cursorPosition = -1;
    float cursorHorizontalOffset = -1;

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

    friend class UISystem;
};
