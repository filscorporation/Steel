#include <vector>

#include "UIText.h"
#include "../../Core/Application.h"
#include "../UIQuadRenderer.h"

void UIText::Rebuild(RectTransformation& rectTransformation, bool transformationDirty)
{
    if (!_dirtyText && !_dirtyTextColor && !transformationDirty)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_font == nullptr || _text.empty())
    {
        ForeachLetterDelete(entitiesRegistry, letters.size());
        _dirtyText = false;
        _dirtyTextColor = false;
        rectTransformation.SetTransformationChanged(false);
        if (_textSizeRef != 0)
        {
            _font->FreeSize(_textSizeRef);
            _textSizeRef = 0;
        }

        return;
    }

    if (rectTransformation.DidSizeChange())
    {
        // If text rect changed in size, completely rebuild
        _lettersChangedCount = letters.size();
        _dirtyText = true;
    }

    if (_dirtyText)
    {
        // If alignment is not to the left, than we need to change all letters positions
        if (!(_textAlignment == AlignmentTypes::TopLeft
           || _textAlignment == AlignmentTypes::CenterLeft
           || _textAlignment == AlignmentTypes::BottomLeft))
        {
            _lettersChangedCount = letters.size();
        }

        // Remove letters that changed
        ForeachLetterDelete(entitiesRegistry, _lettersChangedCount);

        if (letters.empty())
        {
            // If all letter changed, we don't need to change transformation and color, as new one will be created
            transformationDirty = false;
            _dirtyTextColor = false;
        }

        if (_textSizeRef != 0 && _textSizeRef != _textSize)
            _font->FreeSize(_textSizeRef);
        _textSizeRef = _textSize;
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
            ForeachLetterDelete(entitiesRegistry, letters.size());
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
                originY = (float)(-(int)atlas.MinY) / rectSize.y;
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
                cursorX = (uint32_t)std::floor(rectSize.x * 0.5f - (float)fullTextWidth * 0.5f);
                break;
            case AlignmentTypes::TopRight:
            case AlignmentTypes::CenterRight:
            case AlignmentTypes::BottomRight:
                cursorX = (uint32_t)std::floor(rectSize.x - (float)fullTextWidth);
                break;
        }

        // Move cursor to first changed letter
        for (uint32_t i = 0; i < letters.size(); ++i)
            cursorX += atlas.Characters[_text[i]].Advance;
        // Create letter renderer for each letter and calculate its transformation
        letters.reserve(_text.size());
        for (uint32_t i = letters.size(); i < _text.size(); ++i)
        {
            char& c = _text[i];
            auto& character = atlas.Characters[c];

            // If letter doesn't fit into rect, it will be hidden
            bool isRendered = cursorX >= 0 && (float)cursorX + character.Advance <= rectSize.x;
            if (!isRendered)
            {
                letters.push_back(NULL_ENTITY);
                cursorX += character.Advance;
                continue;
            }

            EntityID letterEntityID = entitiesRegistry->CreateNewEntity();
            auto& letterRenderer = entitiesRegistry->AddComponent<UIQuadRenderer>(letterEntityID);

            letterRenderer.Color = _color;
            letterRenderer.TextureID = atlas.TextureID;
            letterRenderer.Queue = RenderingQueue::Text;
            letterRenderer.TextureCoords[0] = glm::vec2(character.TopRight.x, character.BottomLeft.y);
            letterRenderer.TextureCoords[1] = glm::vec2(character.TopRight.x, character.TopRight.y);
            letterRenderer.TextureCoords[2] = glm::vec2(character.BottomLeft.x, character.BottomLeft.y);
            letterRenderer.TextureCoords[3] = glm::vec2(character.BottomLeft.x, character.TopRight.y);

            // Calculate letter rect
            float x = ((float)cursorX + character.Bearing.x + character.Size.x * 0.5f) / rectSize.x - 0.5f;
            float y = originY + (character.Bearing.y - character.Size.y * 0.5f) / rectSize.y - 0.5f;
            float hw = 0.5f * (float)character.Size.x / rectSize.x;
            float hh = 0.5f * (float)character.Size.y / rectSize.y;
            letterRenderer.DefaultVertices[0] = glm::vec4(x + hw, y + hh, 0.0f, 1.0f);
            letterRenderer.DefaultVertices[1] = glm::vec4(x + hw, y - hh, 0.0f, 1.0f);
            letterRenderer.DefaultVertices[2] = glm::vec4(x - hw, y + hh, 0.0f, 1.0f);
            letterRenderer.DefaultVertices[3] = glm::vec4(x - hw, y - hh, 0.0f, 1.0f);

            // Apply text rect transformation
            for (int j = 0; j < 4; ++j)
                letterRenderer.Vertices[j] = rectMatrix * letterRenderer.DefaultVertices[j];

            letters.push_back(letterEntityID);
            cursorX += character.Advance;
        }

        _dirtyText = false;
    }

    if (_dirtyTextColor)
    {
        ForeachLetterChangeColor(entitiesRegistry, _color);
        _dirtyTextColor = false;
    }

    if (transformationDirty)
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

std::string UIText::GetText() const
{
    return _text;
}

void UIText::SetText(const std::string& text)
{
    if (_text == text)
        return;

    // Save how many letters changed (to not rebuild text when one last char changed)
    _lettersChangedCount = letters.size();
    for (uint32_t i = _lettersChangedCount; i < std::min(text.size(), letters.size()); ++i)
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
    if (_textSize == size || size < 1)
        return;

    _lettersChangedCount = letters.size();
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

    _lettersChangedCount = letters.size();
    _textAlignment = alignmentType;
    _dirtyText = true;
}

void UIText::ForeachLetterChangeColor(EntitiesRegistry* registry, glm::vec4 color) const
{
    for (auto& letterID : letters)
    {
        if (letterID == NULL_ENTITY) continue;
        registry->GetComponent<UIQuadRenderer>(letterID).Color = color;
    }
}

void UIText::ForeachLetterDelete(EntitiesRegistry* registry, uint32_t count)
{
    for (uint32_t i = letters.size() - count; i < letters.size(); ++i)
    {
        if (letters[i] == NULL_ENTITY) continue;
        if (registry->EntityExists(letters[i]))
            registry->DeleteEntity(letters[i]);
    }
    letters.resize(letters.size() - count);
}

void UIText::ForeachLetterSetActive(EntitiesRegistry* registry, bool active) const
{
    for (auto& letterID : letters)
    {
        if (letterID == NULL_ENTITY) continue;
        registry->EntitySetActive(letterID, active, false);
    }
}

void UIText::ForeachLetterApplyTransformation(EntitiesRegistry* registry, const glm::mat4& transformationMatrix) const
{
    auto lettersAccessor = registry->GetComponentAccessor<UIQuadRenderer>();
    for (auto& letterID : letters)
    {
        if (letterID == NULL_ENTITY) continue;
        auto& renderer = lettersAccessor.Get(letterID);
        for (int j = 0; j < 4; ++j)
            renderer.Vertices[j] = transformationMatrix * renderer.DefaultVertices[j];
    }
}
