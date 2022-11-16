#pragma once

#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Rendering/Core/RenderContext.h"
#include "Steel/UI/Font.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/RectTransformation.h"

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
    DEFINE_TYPE(UIText)

public:
    explicit UIText(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(UILayer* layer, RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty);
    void Draw(RenderContext* renderContext);
    void Refresh();

    Font* GetFont() const;
    void SetFont(Font* font);
    const std::string& GetText() const;
    void SetText(const std::string& text);
    uint32_t GetTextSize() const;
    void SetTextSize(uint32_t size);
    float GetLineSpacing() const;
    void SetLineSpacing(float spacing);
    const glm::vec4& GetColor() const;
    void SetColor(const glm::vec4& color);
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

    bool IsDirty() const;

private:
    void RebuildInner(RectTransformation& transformation);

    void GetLinesSize(CharactersAtlas& atlas, float maxWidth, std::vector<uint32_t>& linesSize, std::vector<uint32_t>& lettersCount);
    static bool IsNewLine(char c);
    static bool IsSpace(char c);
    int OriginX(float rectWidth, uint32_t lineWidth);
    int OriginY(CharactersAtlas& atlas, float rectHeight, uint32_t textHeight);
    void SetDirty();

    Material* _material = nullptr;
    Font* _font = nullptr;
    std::string _text;
    uint32_t _textSize = 16;
    uint32_t _textSizeRef = 0;
    float _lineSpacing = 1.0f;
    bool _isTextAutoSize = false;
    AlignmentTypes::AlignmentType _textAlignment = AlignmentTypes::CenterLeft;
    OverflowModes::OverflowMode _overflowMode = OverflowModes::Overflow;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    MaterialPropertyBlock _customProperties;
    short _clippingLevel = 0;
    bool isDirtyPublic = true; // Saves dirty state till the next frame

    std::vector<glm::ivec3> lettersDimensions;

    bool isDirty = true;
    VertexBuffer vb;
    IndexBuffer ib;
    float _sortingOrder = 0;
};
