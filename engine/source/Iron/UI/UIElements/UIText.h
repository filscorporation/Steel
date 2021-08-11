#pragma once

#include "../Font.h"
#include "../UIComponent.h"
#include "../RectTransformation.h"
#include "../../Rendering/Material.h"

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

    void Init(EntitiesRegistry* entitiesRegistry);
    void Rebuild(UILayer* layer, RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty);
    void Refresh();

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
    void SetClippingLevel(short clippingLevel);

    glm::vec3 GetLetterOrigin(uint32_t letterIndex, bool& calculated);
    uint32_t GetLetterPosition(const glm::vec2& mousePosition);
    uint32_t GetLetterPositionLocal(const glm::vec2& localPosition);
    uint32_t GetLetterPositionLineUp(uint32_t currentPosition, float& horOffset);
    uint32_t GetLetterPositionLineDown(uint32_t currentPosition, float& horOffset);
    void GetLinesIndices(uint32_t from, uint32_t to, std::vector<std::tuple<uint32_t, uint32_t>>& indices);

    bool IsTextColorDirty() const;
    bool IsTextDirty() const;

private:
    void ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const;
    void ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count);
    void ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const;
    void ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix, float sortingOrder) const;

    void GetLinesSize(CharactersAtlas& atlas, float maxWidth, std::vector<uint32_t>& linesSize, std::vector<uint32_t>& lettersCount);
    static bool IsNewLine(char c);
    static bool IsSpace(char c);
    int OriginX(float rectWidth, uint32_t lineWidth);
    int OriginY(CharactersAtlas& atlas, float rectHeight, uint32_t textHeight);

    Material* _material = nullptr;
    Font* _font = nullptr;
    std::string _text;
    uint32_t _textSize = 14;
    uint32_t _textSizeRef = 0;
    float _lineSpacing = 1.0f;
    bool _isTextAutoSize = false;
    AlignmentTypes::AlignmentType _textAlignment = AlignmentTypes::CenterLeft;
    OverflowModes::OverflowMode _overflowMode = OverflowModes::Overflow;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    short _clippingLevel = 0;

    uint32_t _lettersChangedCount = 0;
    bool _dirtyText = false;
    bool _dirtyTextColor = false;

    std::vector<EntityID> letters;
    std::vector<glm::ivec3> lettersDimensions;

    friend class UISystem;
};
