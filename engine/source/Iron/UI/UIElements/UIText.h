#pragma once

#include "../Font.h"
#include "../UIComponent.h"

namespace AlignmentTypes
{
    enum AlignmentType
    {
        TopLeft,
        TopMiddle,
        TopRight,
        CenterLeft,
        CenterMiddle,
        CenterRight,
        BottomLeft,
        BottomMiddle,
        BottomRight,
    };
}

namespace OverflowModes
{
    enum OverflowMode
    {
        Overflow,
        WrapByLetters,
        WrapByWords,
    };
}

class UIText : public UIComponent
{
public:
    explicit UIText(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    void Rebuild(RectTransformation& transformation, bool transformationDirty);

    Font* GetFont() const;
    void SetFont(Font* font);
    std::string GetText() const;
    uint32_t GetTextSize() const;
    void SetTextSize(uint32_t size);
    float GetLineSpacing() const;
    void SetLineSpacing(float spacing);
    void SetText(const std::string& text);
    const glm::vec4& GetColor() const;
    void SetColor(glm::vec4 color);
    bool GetIsTextAutoSize() const;
    void SetIsTextAutoSize(bool isAutoSize);
    AlignmentTypes::AlignmentType GetTextAlignment() const;
    void SetTextAlignment(AlignmentTypes::AlignmentType alignmentType);
    OverflowModes::OverflowMode GetOverflowMode() const;
    void SetOverflowMode(OverflowModes::OverflowMode overflowMode);

    void SetCursorPosition(uint32_t position);
    void DisableCursor();
    uint32_t GetCursorPosition(const glm::vec2& mousePosition);
    uint32_t GetCursorPositionLocal(const glm::vec2& localPosition);
    uint32_t GetCursorPositionLineUp(uint32_t currentPosition, float& horOffset);
    uint32_t GetCursorPositionLineDown(uint32_t currentPosition, float& horOffset);

private:
    void ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const;
    void ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count);
    void ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const;
    void ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix) const;
    void UpdateCursorColor();

    void GetLinesSize(CharactersAtlas& atlas, float maxWidth, std::vector<uint32_t>& linesSize, std::vector<uint32_t>& lettersCount);
    static bool IsNewLine(char c);
    static bool IsSpace(char c);
    int OriginX(float rectWidth, uint32_t lineWidth);
    int OriginY(CharactersAtlas& atlas, float rectHeight, uint32_t textHeight);
    void DrawCursor(float minY, float maxY, const glm::vec2& rectSize, const glm::mat4& rectMatrix);

    Font* _font = nullptr;
    std::string _text;
    uint32_t _textSize = 14;
    uint32_t _textSizeRef = 0;
    float _lineSpacing = 1.0f;
    bool _isTextAutoSize = false;
    AlignmentTypes::AlignmentType _textAlignment = AlignmentTypes::CenterLeft;
    OverflowModes::OverflowMode _overflowMode = OverflowModes::Overflow;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t _lettersChangedCount = 0;
    bool _dirtyText = false;
    bool _dirtyTextColor = false;

    std::vector<EntityID> letters;
    std::vector<glm::ivec2> lettersDimensions;

    bool drawCursor = false;
    uint32_t cursorPosition = 0;
    EntityID cursor = NULL_ENTITY;
    uint32_t cursorWidth = 2;
    float cursorBlinkRate = 0.53f;
    float cursorBlinkProgress = 0.0f;
    bool cursorIsVisible = true;

    friend class UISystem;
};
