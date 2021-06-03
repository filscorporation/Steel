#include <vector>
#include "UIText.h"

#include "../../Core/Application.h"
#include "../../Core/Log.h"
#include "../../Core/Time.h"

void UIText::Rebuild(RectTransformation& rectTransformation, bool transformationDirty)
{
    if (!_dirtyText && !_dirtyTextColor && !transformationDirty && !drawCursor)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_font == nullptr)
    {
        ForeachLetterDelete(entitiesRegistry, letters.size());
        _dirtyText = false;
        _dirtyTextColor = false;
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
        bool valid = (float)atlas.LineHeight() <= rectSize.y && (float)atlas.MaxX <= rectSize.x;

        // Calculate text metrics
        std::vector<uint32_t> linesSize;
        std::vector<uint32_t> lettersCount;
        if (valid)
        {
            GetLinesSize(atlas, rectSize.x, linesSize, lettersCount);
            valid = !linesSize.empty();
        }

        if (!valid)
        {
            // Text is not valid, it can't be drawn
            ForeachLetterDelete(entitiesRegistry, letters.size());
            _dirtyText = false;
            _dirtyTextColor = false;

            return;
        }
        uint32_t spacing = std::floor((float)atlas.LineHeight() * _lineSpacing);
        uint32_t textHeight = atlas.LineHeight() + (linesSize.size() - 1) * spacing;

        // Calculate alignment variables
        int cursorX = OriginX(rectSize.x, linesSize[0]);
        int cursorY = OriginY(atlas, rectSize.y, textHeight);

        // Move cursor to first changed letter
        // TODO: rework rebuilding optimization for multiline text
        //for (uint32_t i = 0; i < letters.size(); ++i)
        //    cursorX += atlas.Characters[_text[i]].Advance;

        // Create letter renderer for each letter and calculate its transformation
        letters.reserve(_text.size());
        lettersDimensions.reserve(_text.size() + 1);
        uint32_t currentLine = 0, lettersInLine = 0;
        for (uint32_t i = letters.size(); i < _text.size(); ++i)
        {
            char& c = _text[i];
            auto& character = atlas.Characters[c];

            // If letter doesn't fit into rect, it will be hidden
            bool isRendered = cursorX >= 0 && (float)cursorX + (float)character.Advance <= rectSize.x
                && cursorY >= -atlas.MinY && (float)cursorY + (float)atlas.MaxY <= rectSize.y;
            if (isRendered)
            {
                EntityID letterEntityID = entitiesRegistry->CreateNewEntity();
                auto &letterRenderer = entitiesRegistry->AddComponent<UIQuadRenderer>(letterEntityID);

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

                // Save letter origins for text navigation needs
                lettersDimensions.emplace_back(cursorX, cursorY, character.Advance);

                // Apply text rect transformation
                for (int j = 0; j < 4; ++j)
                    letterRenderer.Vertices[j] = rectMatrix * letterRenderer.DefaultVertices[j];

                letters.push_back(letterEntityID);
                lettersInLine++;
                cursorX += (int)character.Advance;
            }
            else
            {
                lettersDimensions.emplace_back(cursorX, cursorY, character.Advance);
                letters.push_back(NULL_ENTITY);
                lettersInLine++;
                cursorX += (int)character.Advance;
            }

            if (lettersInLine >= lettersCount[currentLine] && currentLine < linesSize.size() - 1)
            {
                currentLine++;
                lettersInLine = 0;
                cursorX = OriginX(rectSize.x, currentLine >= linesSize.size() ? 0 : linesSize[currentLine]);
                cursorY -= (int)spacing;
            }
        }

        // Add one more letter point in the end for new letters
        lettersDimensions.emplace_back(cursorX, cursorY, 0);

        _dirtyText = false;
    }

    if (_dirtyTextColor)
    {
        ForeachLetterChangeColor(entitiesRegistry, _color);
        UpdateCursorColor();
        _dirtyTextColor = false;
    }

    if (transformationDirty)
    {
        auto& rectMatrix = rectTransformation.GetTransformationMatrixCached();
        ForeachLetterApplyTransformation(entitiesRegistry, rectMatrix);
    }

    if (drawCursor)
    {
        auto& atlas = _font->characters[_textSize];
        auto rectSize = rectTransformation.GetRealSizeCached();
        auto& rectMatrix = rectTransformation.GetTransformationMatrixCached();
        DrawCursor((float) atlas.MinY, (float) atlas.MaxY, rectSize, rectMatrix);
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

void UIText::SetCursorPosition(uint32_t position)
{
    drawCursor = true;

    cursorBlinkProgress = 0;
    cursorIsInvisible = false;
    cursorPosition = position;
}

void UIText::DisableCursor()
{
    if (!drawCursor)
        return;
    drawCursor = false;
    if (cursor != NULL_ENTITY)
        Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(cursor, false, true);
}

uint32_t UIText::GetCursorPosition(const glm::vec2& mousePosition)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& atlas = _font->characters[_textSize];
    float minY = atlas.MinY, maxY = atlas.MaxY;

    glm::mat4 transformation = entitiesRegistry->GetComponent<RectTransformation>(Owner).GetTransformationMatrixCached();
    glm::vec2 rectSize = entitiesRegistry->GetComponent<RectTransformation>(Owner).GetRealSizeCached();
    // Transform mouse position to text local coord system
    glm::vec4 localPosition = glm::inverse(transformation) * glm::vec4(mousePosition, 0.0f, 1.0f);
    // Then transform it to pixel scale in the rect (same as cached letters positions)
    localPosition = (localPosition + glm::vec4(0.5f)) * glm::vec4(rectSize, 1.0f, 1.0f);

    // TODO: binary search
    int i;
    float minYDist = 0;
    int minYIndex = -1;
    for (i = 0; i < lettersDimensions.size(); ++i)
    {
        if (localPosition.y <= (float)lettersDimensions[i].y + maxY
            && localPosition.y >= (float)lettersDimensions[i].y + minY)
            break;
        float dist = std::abs(localPosition.y - (float)lettersDimensions[i].y);
        if (minYIndex == -1 || dist < minYDist)
        {
            minYIndex = i;
            minYDist = dist;
        }
    }
    if (i >= lettersDimensions.size())
        i = minYIndex;

    float minXDist = 0;
    int minXIndex = -1;
    for (int j = 0; j < lettersDimensions.size(); ++j)
    {
        if (lettersDimensions[i].y != lettersDimensions[j].y)
            continue;

        float dist = std::abs(localPosition.x - (float)lettersDimensions[j].x);
        if (minXIndex == -1 || dist < minXDist)
        {
            minXIndex = j;
            minXDist = dist;
        }
    }

    if (minXIndex == -1)
    {
        // This should never happen
        return 0;
    }

    return minXIndex;
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
    lettersDimensions.resize(letters.size());
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

void UIText::UpdateCursorColor()
{
    if (cursor == NULL_ENTITY)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& cursorRenderer = entitiesRegistry->GetComponent<UIQuadRenderer>(cursor);
    cursorRenderer.Color = _color;
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
                    if (lettersCount[line] != 0 && !IsSpace(_text[i]) && lastSpace != -1)
                    {
                        i -= (int)(lettersCount[line] - lastSpace - 1);
                        linesSize[line] -= wordSize;
                        lettersCount[line] = lastSpace + 1;
                    }
                    if (IsSpace(_text[i]))
                    {
                        // Skip space at the start of the next line
                        i++;
                        lettersCount[line]++;
                    }
                    lastSpace = -1;
                    wordSize = 0;
                    linesSize.push_back(0);
                    lettersCount.push_back(0);
                    line++;
                    i--;
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

    return 0;
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

    return 0;
}

void UIText::DrawCursor(float minY, float maxY, const glm::vec2& rectSize, const glm::mat4& rectMatrix)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (cursor == NULL_ENTITY)
    {
        // Create cursor renderer if it was not already
        auto cursorSprite = Application::Instance->GetCurrentScene()->GetUILayer()->UIResources.DefaultPixelSprite;
        cursor = entitiesRegistry->CreateNewEntity();

        auto& cursorRenderer = entitiesRegistry->AddComponent<UIQuadRenderer>(cursor);
        cursorRenderer.Color = _color;
        cursorRenderer.TextureID = cursorSprite->TextureID;
        cursorRenderer.Queue = RenderingQueue::Text;
        cursorRenderer.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
        cursorRenderer.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
        cursorRenderer.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
        cursorRenderer.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
    }

    cursorBlinkProgress += Time::UnscaledDeltaTime();
    if (cursorBlinkProgress > cursorBlinkRate)
    {
        cursorBlinkProgress -= cursorBlinkRate;
        cursorIsInvisible = !cursorIsInvisible;
        entitiesRegistry->EntitySetActive(cursor, !cursorIsInvisible, true);
    }

    if (cursorIsInvisible)
        return;

    float width = (float)cursorWidth / rectSize.x;
    uint32_t realPosition = std::min(letters.size(), cursorPosition);

    // Place cursor in the origin of the letter where it is positioned
    float ox = (float)lettersDimensions[realPosition].x / rectSize.x - 0.5f;
    float oy = (float)lettersDimensions[realPosition].y / rectSize.y - 0.5f;

    float up = maxY / rectSize.y;
    float down = minY / rectSize.y;

    bool isRendered = ox >= -0.5f && ox <= 0.5f && oy + down >= -0.5f && oy + up <= 0.5f;
    if (!isRendered)
    {
        entitiesRegistry->EntitySetActive(cursor, false, true);
        return;
    }

    entitiesRegistry->EntitySetActive(cursor, true, true);
    auto& cursorRenderer = entitiesRegistry->GetComponent<UIQuadRenderer>(cursor);
    cursorRenderer.DefaultVertices[0] = glm::vec4(ox + width, oy + up, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[1] = glm::vec4(ox + width, oy + down, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[2] = glm::vec4(ox, oy + up, 0.0f, 1.0f);
    cursorRenderer.DefaultVertices[3] = glm::vec4(ox, oy + down, 0.0f, 1.0f);

    for (int j = 0; j < 4; ++j)
        cursorRenderer.Vertices[j] = rectMatrix * cursorRenderer.DefaultVertices[j];
}
