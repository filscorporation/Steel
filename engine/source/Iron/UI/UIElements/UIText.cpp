#include <vector>

#include "UIText.h"
#include "../../Core/Application.h"
#include "../../Core/Log.h"
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
        std::vector<uint32_t> linesSize;
        std::vector<uint32_t> lettersCount;
        GetLinesSize(atlas, rectSize.x, linesSize, lettersCount);
        uint32_t spacing = std::floor((float)atlas.LineHeight() * _lineSpacing);
        uint32_t textHeight = atlas.LineHeight() + (linesSize.size() - 1) * spacing;

        if (linesSize.empty() || (float)atlas.LineHeight() > rectSize.y)
        {
            // Text height is bigger than rect, text can't be drawn
            ForeachLetterDelete(entitiesRegistry, letters.size());
            _dirtyText = false;
            _dirtyTextColor = false;
            rectTransformation.SetTransformationChanged(false);

            return;
        }

        // Calculate alignment variables
        int cursorX = OriginX(rectSize.x, linesSize[0]);
        int cursorY = OriginY(atlas, rectSize.y, textHeight);

        // Move cursor to first changed letter
        // TODO: rework rebuilding optimization for multiline text
        //for (uint32_t i = 0; i < letters.size(); ++i)
        //    cursorX += atlas.Characters[_text[i]].Advance;

        // Create letter renderer for each letter and calculate its transformation
        letters.reserve(_text.size());
        uint32_t currentLine = 0, lettersInLine = 0;
        for (uint32_t i = letters.size(); i < _text.size(); ++i)
        {
            if (lettersInLine >= lettersCount[currentLine])
            {
                currentLine++;
                lettersInLine = 0;
                cursorX = OriginX(rectSize.x, linesSize[currentLine]);
                cursorY -= (int)spacing;
                i--;
                continue;
            }

            char& c = _text[i];
            auto& character = atlas.Characters[c];

            // If letter doesn't fit into rect, it will be hidden
            bool isRendered = cursorX >= 0 && (float)cursorX + (float)character.Advance <= rectSize.x;
            if (!isRendered)
            {
                letters.push_back(NULL_ENTITY);
                lettersInLine++;
                cursorX += (int)character.Advance;
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
            float x = ((float)cursorX + (float)character.Bearing.x + (float)character.Size.x * 0.5f) / rectSize.x - 0.5f;
            float y = ((float)cursorY + (float)character.Bearing.y - (float)character.Size.y * 0.5f) / rectSize.y - 0.5f;
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
            lettersInLine++;
            cursorX += (int)character.Advance;
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
    // TODO: rework rebuilding optimization for multiline text
    _lettersChangedCount = letters.size();
    //for (uint32_t i = _lettersChangedCount; i < std::min(text.size(), letters.size()); ++i)
    //{
    //    if (text[i] == _text[i])
    //        _lettersChangedCount--;
    //    else
    //        break;
    //}

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

float UIText::GetLineSpacing() const
{
    return _lineSpacing;
}

void UIText::SetLineSpacing(float spacing)
{
    if (_lineSpacing == spacing || spacing < 0.0f)
        return;

    _lettersChangedCount = letters.size();
    _lineSpacing = spacing;
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

OverflowModes::OverflowMode UIText::GetOverflowMode() const
{
    return _overflowMode;
}

void UIText::SetOverflowMode(OverflowModes::OverflowMode overflowMode)
{
    if (_overflowMode == overflowMode)
        return;

    _lettersChangedCount = letters.size();
    _overflowMode = overflowMode;
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

bool UIText::IsNewLine(char c)
{
    return c == '\n';
}

bool UIText::IsSpace(char c)
{
    return c == ' ';
}

void UIText::GetLinesSize(CharactersAtlas& atlas, float maxWidth, std::vector<uint32_t>& linesSize, std::vector<uint32_t>& lettersCount)
{
    int line = 0;
    linesSize.push_back(0);
    lettersCount.push_back(0);
    int lastSpace = -1;
    uint32_t wordSize = 0;

    for (int i = 0; i < _text.size(); ++i)
    {
        if (IsNewLine(_text[i]))
        {
            lastSpace = -1;
            linesSize.push_back(0);
            lettersCount[line]++;
            lettersCount.push_back(0);
            line++;
            continue;
        }
        auto& character = atlas.Characters[_text[i]];
        switch (_overflowMode)
        {
            case OverflowModes::Overflow:
                break;
            case OverflowModes::WrapByLetters:
                if (linesSize[line] + character.Advance > maxWidth)
                {
                    lastSpace = -1;
                    wordSize = 0;
                    linesSize.push_back(0);
                    lettersCount.push_back(0);
                    line++;
                    i--;
                    continue;
                }
                break;
            case OverflowModes::WrapByWords:
                if (linesSize[line] + character.Advance > maxWidth)
                {
                    if (!IsSpace(_text[i]) && lastSpace != -1)
                    {
                        i -= (int)(lettersCount[line] - lastSpace);
                        linesSize[line] -= wordSize;
                        lettersCount[line] = lastSpace;
                    }
                    lastSpace = -1;
                    wordSize = 0;
                    linesSize.push_back(0);
                    lettersCount.push_back(0);
                    line++;
                    i--; // TODO: next line will start with space
                    continue;
                }
                break;
        }
        linesSize[line] += character.Advance;
        lettersCount[line]++;
        if (IsSpace(_text[i]))
        {
            lastSpace = lettersCount[line] - 1;
            wordSize = 0;
        }
        else
        {
            wordSize += character.Advance;
        }
    }
}

int UIText::OriginX(float rectWidth, uint32_t lineWidth)
{
    switch (_textAlignment)
    {
        case AlignmentTypes::TopLeft:
        case AlignmentTypes::CenterLeft:
        case AlignmentTypes::BottomLeft:
            return 0;
        case AlignmentTypes::TopMiddle:
        case AlignmentTypes::CenterMiddle:
        case AlignmentTypes::BottomMiddle:
            return (int)std::floor((rectWidth - (float)lineWidth) * 0.5f);
        case AlignmentTypes::TopRight:
        case AlignmentTypes::CenterRight:
        case AlignmentTypes::BottomRight:
            return (int)std::floor(rectWidth - (float)lineWidth);
    }
}

int UIText::OriginY(CharactersAtlas& atlas, float rectHeight, uint32_t textHeight)
{
    switch (_textAlignment)
    {
        case AlignmentTypes::TopLeft:
        case AlignmentTypes::TopMiddle:
        case AlignmentTypes::TopRight:
            return (int)std::floor(rectHeight - (float)atlas.MaxY);
        case AlignmentTypes::CenterLeft:
        case AlignmentTypes::CenterMiddle:
        case AlignmentTypes::CenterRight:
            return (int)std::floor((rectHeight + (float)textHeight) * 0.5f) - (int)atlas.MaxY;
        case AlignmentTypes::BottomLeft:
        case AlignmentTypes::BottomMiddle:
        case AlignmentTypes::BottomRight:
            return (int)textHeight - (int)atlas.MaxY;
    }
}
