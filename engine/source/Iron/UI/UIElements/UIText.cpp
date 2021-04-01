#include "UIText.h"
#include "../UIRenderer.h"
#include "../../Core/Application.h"
#include "../../Scene/SceneHelper.h"

void UIText::Rebuild()
{
    auto& rectTransformation = GetComponentS<RectTransformation>(Owner);
    bool dirtyTransform = rectTransformation.DidTransformationChange();
    if (!_dirtyText && !_dirtyTextColor && !dirtyTransform)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_font == nullptr || _text.empty())
    {
        ForeachLetterDelete(entitiesRegistry, lettersCount);
        lastLetterID = NULL_ENTITY;
        lettersCount = 0;
        _dirtyText = false;
        _dirtyTextColor = false;
        rectTransformation.SetTransformationChanged(false);

        return;
    }

    auto newSize = rectTransformation.GetRealSizeCached();
    if (std::abs(newSize.x  -_lastRectSize.x) > 0.001f || std::abs(newSize.y  -_lastRectSize.y) > 0.001f)
    {
        // If text rect changed in size, completely rebuild
        _lettersChangedCount = lettersCount;
        _dirtyText = true;
        _lastRectSize = newSize;
    }

    if (_dirtyText)
    {
        // If alignment is not to the left, than we need to change all letters positions
        if (!(_textAlignment == AlignmentTypes::TopLeft
           || _textAlignment == AlignmentTypes::CenterLeft
           || _textAlignment == AlignmentTypes::BottomLeft))
        {
            _lettersChangedCount = lettersCount;
        }

        // Remove letters that changed
        lastLetterID = ForeachLetterDelete(entitiesRegistry, _lettersChangedCount);
        lettersCount -= _lettersChangedCount;

        if (lettersCount == 0)
        {
            // If all letter changed, we don't need to change transformation and color, as new one will be created
            dirtyTransform = false;
            _dirtyTextColor = false;
        }

        _font->AddSizeIfNotExists(_textSize);
        auto& atlas = _font->characters[_textSize];
        auto rectSize = rectTransformation.GetRealSizeCached();
        auto& rectMatrix = rectTransformation.GetTransformationMatrixCached();

        // We will try to fit text into the rect

        // Calculate text metrics
        int fullTextWidth = 0;
        for (auto c : _text)
        {
            auto& character = atlas.Characters[c];
            fullTextWidth += character.Advance;
        }

        if ((float)(atlas.MaxY - atlas.MinY) > rectSize.y)
        {
            // Text height is bigger than rect, text can't be drawn
            ForeachLetterDelete(entitiesRegistry, lettersCount);
            lastLetterID = NULL_ENTITY;
            lettersCount = 0;
            _dirtyText = false;
            _dirtyTextColor = false;
            rectTransformation.SetTransformationChanged(false);

            return;
        }

        // Calculate alignment variables
        float originY = 0;
        switch (_textAlignment)
        {
            case AlignmentTypes::TopLeft:
            case AlignmentTypes::TopMiddle:
            case AlignmentTypes::TopRight:
                originY = (rectSize.y - atlas.MaxY) / rectSize.y;
                break;
            case AlignmentTypes::CenterLeft:
            case AlignmentTypes::CenterMiddle:
            case AlignmentTypes::CenterRight:
                originY = 0.5f - (float)(atlas.MaxY + atlas.MinY) * 0.5f / rectSize.y;
                break;
            case AlignmentTypes::BottomLeft:
            case AlignmentTypes::BottomMiddle:
            case AlignmentTypes::BottomRight:
                originY = (float)(-atlas.MinY) / rectSize.y;
                break;
        }

        uint32_t cursorX = 0;
        switch (_textAlignment)
        {
            case AlignmentTypes::TopLeft:
            case AlignmentTypes::CenterLeft:
            case AlignmentTypes::BottomLeft:
                cursorX = 0;
                break;
            case AlignmentTypes::TopMiddle:
            case AlignmentTypes::CenterMiddle:
            case AlignmentTypes::BottomMiddle:
                cursorX = std::floor(rectSize.x * 0.5f - (float)fullTextWidth * 0.5f);
                break;
            case AlignmentTypes::TopRight:
            case AlignmentTypes::CenterRight:
            case AlignmentTypes::BottomRight:
                cursorX = std::floor(rectSize.x - (float)fullTextWidth);
                break;
        }

        // Move cursor to first changed letter
        EntityID last = lastLetterID;
        for (int i = 0; i < lettersCount; ++i)
            cursorX += atlas.Characters[_text[i]].Advance;
        // Create letter renderer for each letter and calculate its transformation
        for (int i = lettersCount; i < _text.size(); ++i)
        {
            wchar_t& c = _text[i];
            auto& character = atlas.Characters[c];

            EntityID letterEntityID = entitiesRegistry->CreateNewEntity();
            auto& letterRenderer = entitiesRegistry->AddComponent<UILetterRenderer>(letterEntityID);

            // If letter doesn't fit into rect, it will be hidden
            letterRenderer.IsRendered = cursorX >= 0 && (float)cursorX + character.Advance <= rectSize.x;
            letterRenderer.PreviousLetter = last;
            letterRenderer.Color = _color;
            letterRenderer.TextureID = atlas.TextureID;
            letterRenderer.TextureCoords[0] = glm::vec2(character.TopRight.x, character.BottomLeft.y);
            letterRenderer.TextureCoords[1] = glm::vec2(character.TopRight.x, character.TopRight.y);
            letterRenderer.TextureCoords[2] = glm::vec2(character.BottomLeft.x, character.BottomLeft.y);
            letterRenderer.TextureCoords[3] = glm::vec2(character.BottomLeft.x, character.TopRight.y);

            // Calculate letter rect
            float x = ((float)cursorX + character.Bearing.x + character.Size.x * 0.5f) / rectSize.x - 0.5f;
            float y = originY + (character.Bearing.y - character.Size.y * 0.5f) / rectSize.y - 0.5f;
            float hw = 0.5f * (float)character.Size.x / rectSize.x;
            float hh = 0.5f * (float)character.Size.y / rectSize.y;
            letterRenderer.Vertices[0] = glm::vec4(x + hw, y + hh, 0.0f, 1.0f);
            letterRenderer.Vertices[1] = glm::vec4(x + hw, y - hh, 0.0f, 1.0f);
            letterRenderer.Vertices[2] = glm::vec4(x - hw, y + hh, 0.0f, 1.0f);
            letterRenderer.Vertices[3] = glm::vec4(x - hw, y - hh, 0.0f, 1.0f);

            // Apply text rect transformation
            letterRenderer.QuadCache.Vertices[0] = rectMatrix * letterRenderer.Vertices[0];
            letterRenderer.QuadCache.Vertices[1] = rectMatrix * letterRenderer.Vertices[1];
            letterRenderer.QuadCache.Vertices[2] = rectMatrix * letterRenderer.Vertices[2];
            letterRenderer.QuadCache.Vertices[3] = rectMatrix * letterRenderer.Vertices[3];

            last = letterEntityID;
            cursorX += character.Advance;
        }

        lastLetterID = last;
        lettersCount = _text.size();
        _dirtyText = false;
    }

    if (_dirtyTextColor)
    {
        ForeachLetterChangeColor(entitiesRegistry, _color);
        _dirtyTextColor = false;
    }

    if (dirtyTransform)
    {
        auto& rectMatrix = rectTransformation.GetTransformationMatrixCached();
        ForeachLetterApplyTransformation(entitiesRegistry, rectMatrix);
        rectTransformation.SetTransformationChanged(false);
    }
}

Font* UIText::GetFont() const
{
    return _font;
}

void UIText::SetFont(Font* font)
{
    _font = font;

    _dirtyText = true;
}

std::wstring UIText::GetText() const
{
    return _text;
}

void UIText::SetText(const std::wstring& text)
{
    if (_text == text)
        return;

    // Save how many letters changed (to not rebuild text when one last char changed)
    _lettersChangedCount = lettersCount;
    for (int i = 0; i < std::min(text.size(), lettersCount); ++i)
    {
        if (text[i] == _text[i])
            _lettersChangedCount--;
        else
            break;
    }

    _text = text;
    _dirtyText = true;
}

uint32_t UIText::GetTextSize() const
{
    return _textSize;
}

void UIText::SetTextSize(uint32_t size)
{
    if (_textSize == size)
        return;

    _lettersChangedCount = lettersCount;
    _textSize = size;
    _dirtyText = true;
}

const glm::vec4& UIText::GetColor() const
{
    return _color;
}

void UIText::SetColor(glm::vec4 color)
{
    if (_color == color)
        return;

    _color = color;
    _dirtyTextColor = true;
}

bool UIText::GetIsTextAutoSize() const
{
    return _isTextAutoSize;
}

void UIText::SetIsTextAutoSize(bool isAutoSize)
{
    if (_isTextAutoSize == isAutoSize)
        return;

    _isTextAutoSize = isAutoSize;
    _dirtyText = true;
}

AlignmentTypes::AlignmentType UIText::GetTextAlignment() const
{
    return _textAlignment;
}

void UIText::SetTextAlignment(AlignmentTypes::AlignmentType alignmentType)
{
    if (_textAlignment == alignmentType)
        return;

    _lettersChangedCount = lettersCount;
    _textAlignment = alignmentType;
    _dirtyText = true;
}

void UIText::ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const
{
    EntityID currentLetterID = lastLetterID;
    for (uint32_t i = 0; i < lettersCount; ++i)
    {
        auto& currentLetter = registry->GetComponent<UILetterRenderer>(currentLetterID);
        currentLetter.Color = color;

        currentLetterID = currentLetter.PreviousLetter;
    }
}

EntityID UIText::ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count) const
{
    EntityID currentLetterID = lastLetterID;
    auto lettersAccessor = registry->GetComponentAccessor<UILetterRenderer>();
    for (uint32_t i = 0; i < count; ++i)
    {
        if (!lettersAccessor.Has(currentLetterID))
            return NULL_ENTITY;

        auto& currentLetter = lettersAccessor.Get(currentLetterID);
        currentLetterID = currentLetter.PreviousLetter;

        if (registry->EntityExists(currentLetter.Owner))
            registry->DeleteEntity(currentLetter.Owner);
    }

    return currentLetterID;
}

void UIText::ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const
{
    EntityID currentLetterID = lastLetterID;
    auto lettersAccessor = registry->GetComponentAccessor<UILetterRenderer>();
    for (uint32_t i = 0; i < lettersCount; ++i)
    {
        auto& currentLetter = active ? lettersAccessor.GetInactive(currentLetterID) : lettersAccessor.Get(currentLetterID);
        currentLetterID = currentLetter.PreviousLetter;

        registry->EntitySetActive(currentLetter.Owner, active, false);
    }
}

void UIText::ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix) const
{
    EntityID currentLetterID = lastLetterID;
    auto lettersAccessor = registry->GetComponentAccessor<UILetterRenderer>();
    for (uint32_t i = 0; i < lettersCount; ++i)
    {
        auto& currentLetter = lettersAccessor.Get(currentLetterID);
        for (int j = 0; j < 4; ++j)
        {
            currentLetter.QuadCache.Vertices[j] = transformationMatrix * currentLetter.Vertices[j];
        }

        currentLetterID = currentLetter.PreviousLetter;
    }
}
