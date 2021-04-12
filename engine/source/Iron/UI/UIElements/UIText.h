#pragma once

#include "../Font.h"
#include "../UIComponent.h"
#include "../UILetterRenderer.h"

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

class UIText : public UIComponent
{
public:
    explicit UIText(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    void Rebuild();

    Font* GetFont() const;
    void SetFont(Font* font);
    std::wstring GetText() const;
    uint32_t GetTextSize() const;
    void SetTextSize(uint32_t size);
    void SetText(const std::wstring& text);
    const glm::vec4& GetColor() const;
    void SetColor(glm::vec4 color);
    bool GetIsTextAutoSize() const;
    void SetIsTextAutoSize(bool isAutoSize);
    AlignmentTypes::AlignmentType GetTextAlignment() const;
    void SetTextAlignment(AlignmentTypes::AlignmentType alignmentType);

private:
    void ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const;
    EntityID ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count) const;
    void ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const;
    void ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix) const;

    Font* _font = nullptr;
    std::wstring _text;
    uint32_t _textSize = 14;
    uint32_t _textSizeRef = 0;
    bool _isTextAutoSize = false;
    AlignmentTypes::AlignmentType _textAlignment = AlignmentTypes::CenterLeft;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t _lettersChangedCount = 0;
    bool _dirtyText = false;
    bool _dirtyTextColor = false;
    glm::vec2 _lastRectSize = glm::vec2(0.0f, 0.0f);

    EntityID lastLetterID = NULL_ENTITY;
    uint32_t lettersCount = 0;

    friend class UISystem;
};
