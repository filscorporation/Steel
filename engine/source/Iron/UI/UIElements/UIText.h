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
    void SetText(const std::string& text);
    const glm::vec4& GetColor() const;
    void SetColor(glm::vec4 color);
    bool GetIsTextAutoSize() const;
    void SetIsTextAutoSize(bool isAutoSize);
    AlignmentTypes::AlignmentType GetTextAlignment() const;
    void SetTextAlignment(AlignmentTypes::AlignmentType alignmentType);

private:
    void ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const;
    void ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count);
    void ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const;
    void ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix) const;

    Font* _font = nullptr;
    std::string _text;
    uint32_t _textSize = 14;
    uint32_t _textSizeRef = 0;
    bool _isTextAutoSize = false;
    AlignmentTypes::AlignmentType _textAlignment = AlignmentTypes::CenterLeft;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t _lettersChangedCount = 0;
    bool _dirtyText = false;
    bool _dirtyTextColor = false;
    glm::vec2 _lastRectSize = glm::vec2(0.0f, 0.0f);

    std::vector<EntityID> letters;

    friend class UISystem;
};
