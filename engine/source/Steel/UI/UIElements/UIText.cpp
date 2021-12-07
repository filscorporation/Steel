#include <vector>
#include <cctype>

#include "UIText.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/Hierarchy.h"
#include "Steel/Scene/SceneHelper.h"

bool UIText::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIText::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    _font = Application::Instance->GetResourcesManager()->DefaultFont();
    _clippingLevel = GetClippingLevelUpwards(entitiesRegistry, Owner);
}

void UIText::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    vb.Clear();
    ib.Clear();
}

void UIText::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    SetDirty();
}

void UIText::Rebuild(UILayer* layer, RectTransformation& rectTransformation, bool transformationDirty, bool sortingOrderDirty)
{
    if (transformationDirty || sortingOrderDirty || isDirty)
        RebuildInner(rectTransformation);
}

void UIText::Draw(RenderContext* renderContext)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    if (vb.IsEmpty() || ib.IsEmpty())
        return;

    DrawCall drawCall;
    drawCall.VB = vb;
    drawCall.IB = ib;
    drawCall.RenderMaterial = _material;
    drawCall.CustomProperties = _customProperties;
    drawCall.SortingOrder = _sortingOrder;
    drawCall.Queue = RenderingQueue::Opaque;

    renderContext->List.AddDrawCall(drawCall);
}

void UIText::Refresh()
{
    isDirtyPublic = false;
}

Font* UIText::GetFont() const
{
    return _font;
}

void UIText::SetFont(Font* font)
{
    _font = font;
    SetDirty();
}

std::string UIText::GetText() const
{
    return _text;
}

void UIText::SetText(const std::string& text)
{
    if (_text == text)
        return;

    _text = text;
    SetDirty();
}

uint32_t UIText::GetTextSize() const
{
    return _textSize;
}

void UIText::SetTextSize(uint32_t size)
{
    if (_textSize == size || size < 1)
        return;

    _textSize = size;
    SetDirty();
}

float UIText::GetLineSpacing() const
{
    return _lineSpacing;
}

void UIText::SetLineSpacing(float spacing)
{
    if (_lineSpacing == spacing || spacing < 0.0f)
        return;

    _lineSpacing = spacing;
    SetDirty();
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
    SetDirty();
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
    SetDirty();
}

AlignmentTypes::AlignmentType UIText::GetTextAlignment() const
{
    return _textAlignment;
}

void UIText::SetTextAlignment(AlignmentTypes::AlignmentType alignmentType)
{
    if (_textAlignment == alignmentType)
        return;

    _textAlignment = alignmentType;
    SetDirty();
}

OverflowModes::OverflowMode UIText::GetOverflowMode() const
{
    return _overflowMode;
}

void UIText::SetOverflowMode(OverflowModes::OverflowMode overflowMode)
{
    if (_overflowMode == overflowMode)
        return;

    _overflowMode = overflowMode;
    SetDirty();
}

void UIText::SetClippingLevel(short clippingLevel)
{
    if (clippingLevel == _clippingLevel)
        return;

    _clippingLevel = clippingLevel;
    SetDirty();
}

glm::vec3 UIText::GetLetterOrigin(uint32_t letterIndex, bool& calculated)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    calculated = letterIndex < lettersDimensions.size();
    if (!calculated)
        return glm::vec3(0.0f);

    return lettersDimensions[letterIndex];
}

uint32_t UIText::GetLetterPosition(const glm::vec2& mousePosition)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    glm::mat4 transformation = entitiesRegistry->GetComponent<RectTransformation>(Owner).GetTransformationMatrixCached();
    glm::vec2 rectSize = entitiesRegistry->GetComponent<RectTransformation>(Owner).GetRealSizeCached();
    // Transform mouse position to text local coord system
    glm::vec4 localPosition = glm::inverse(transformation) * glm::vec4(mousePosition, 0.0f, 1.0f);
    // Then transform it to pixel scale in the rect (same as cached letters positions)
    localPosition = (localPosition + glm::vec4(0.5f)) * glm::vec4(rectSize, 1.0f, 1.0f);

    return GetLetterPositionLocal(localPosition);
}

uint32_t UIText::GetLetterPositionLocal(const glm::vec2& localPosition)
{
    auto& atlas = _font->characters[_textSize];
    float minY = atlas.MinY, maxY = atlas.MaxY;

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

uint32_t UIText::GetLetterPositionLineUp(uint32_t currentPosition, float& horOffset)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    auto& atlas = _font->characters[_textSize];
    float height = _lineSpacing * (float)(atlas.MaxY - atlas.MinY);

    uint32_t realPosition = std::min((uint32_t)_text.size(), currentPosition);

    for (auto& letterDimension : lettersDimensions)
    {
        if (letterDimension.y > lettersDimensions[realPosition].y)
        {
            if (horOffset < 0)
                horOffset = (float)lettersDimensions[realPosition].x;
            glm::vec2 localPosition = glm::vec2(horOffset, lettersDimensions[realPosition].y);
            localPosition = localPosition + glm::vec2(0, height);
            return GetLetterPositionLocal(localPosition);
        }
    }

    return 0;
}

uint32_t UIText::GetLetterPositionLineDown(uint32_t currentPosition, float& horOffset)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    auto& atlas = _font->characters[_textSize];
    float height = _lineSpacing * (float)(atlas.MaxY - atlas.MinY);

    uint32_t realPosition = std::min((uint32_t)_text.size(), currentPosition);

    for (auto& letterDimension : lettersDimensions)
    {
        if (letterDimension.y < lettersDimensions[realPosition].y)
        {
            if (horOffset < 0)
                horOffset = (float)lettersDimensions[realPosition].x;
            glm::vec2 localPosition = glm::vec2(horOffset, lettersDimensions[realPosition].y);
            localPosition = localPosition - glm::vec2(0, height);
            return GetLetterPositionLocal(localPosition);
        }
    }

    return _text.size();
}

void UIText::GetLinesIndices(uint32_t from, uint32_t to, std::vector<std::tuple<uint32_t, uint32_t>>& indices)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    if (lettersDimensions.size() < to)
        return;

    // TODO: probably should just be cached with letters origins
    uint32_t lastY = lettersDimensions[from].y;
    uint32_t lastIndex = from;
    for (uint32_t i = from + 1; i <= to; ++i)
    {
        uint32_t currentY = lettersDimensions[i].y;
        if (currentY != lastY)
        {
            lastY = currentY;
            indices.emplace_back(lastIndex, i - 1);
            lastIndex = i;
        }
    }
    indices.emplace_back(lastIndex, to);
}

bool UIText::IsDirty() const
{
    return isDirtyPublic;
}

void UIText::SetDirty()
{
    // TODO: maybe replace with event subscription
    isDirty = true;
    isDirtyPublic = true;
}

void UIText::RebuildInner(RectTransformation& transformation)
{
    isDirty = false;

    _sortingOrder = transformation.GetSortingOrder();

    if (_material == nullptr)
        _material = Application::Instance->GetResourcesManager()->DefaultUIMaterial();

    ib.Clear();
    vb.Clear();
    lettersDimensions.clear();

    if (_font == nullptr)
    {
        if (_textSizeRef != 0)
        {
            _font->FreeSize(_textSizeRef);
            _textSizeRef = 0;
        }

        return;
    }

    if (_textSizeRef != 0 && _textSizeRef != _textSize)
        _font->FreeSize(_textSizeRef);
    _textSizeRef = _textSize;
    _font->AddSizeIfNotExists(_textSize);
    auto& atlas = _font->characters[_textSize];

    _customProperties.SetTexture(MAIN_TEX, atlas.TextureID);
    _customProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
    _customProperties.UpdateHash();

    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    auto rectSize = transformation.GetRealSizeCached();

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
        return;
    }

    uint32_t spacing = std::floor((float)atlas.LineHeight() * _lineSpacing);
    uint32_t textHeight = atlas.LineHeight() + (linesSize.size() - 1) * spacing;

    // Calculate alignment variables
    int cursorX = OriginX(rectSize.x, linesSize[0]);
    int cursorY = OriginY(atlas, rectSize.y, textHeight);

    // Count letters to render (preprocess all calculations to allocate the right amount of memory from the first try)
    uint32_t renderedLettersCount = 0;
    uint32_t currentLine = 0, lettersInLine = 0;
    for (char& c : _text)
    {
        auto& character = atlas.Characters[c];

        // If letter doesn't fit into rect, it will be hidden
        bool isRendered = cursorX >= 0 && (float)cursorX + (float)character.Advance <= rectSize.x
                          && cursorY >= -atlas.MinY && (float)cursorY + (float)atlas.MaxY <= rectSize.y;
        if (isRendered)
            renderedLettersCount++;

        lettersInLine++;
        cursorX += (int)character.Advance;

        if (lettersInLine >= lettersCount[currentLine] && currentLine < linesSize.size() - 1)
        {
            currentLine++;
            lettersInLine = 0;
            cursorX = OriginX(rectSize.x, linesSize[currentLine]);
            cursorY -= (int)spacing;
        }
    }

    if (renderedLettersCount == 0)
    {
        // No letters to draw
        return;
    }

    // Create vertices and indices for each letter
    const uint32_t verticesSize = 9 * 4 * renderedLettersCount; // 4 vertices for each letter
    auto vertices = new float[verticesSize];
    const uint32_t indicesSize = 6 * renderedLettersCount; // 2 triangles for each letter
    auto indices = new uint32_t[indicesSize];

    cursorX = OriginX(rectSize.x, linesSize[0]);
    cursorY = OriginY(atlas, rectSize.y, textHeight);

    lettersDimensions.reserve(_text.size() + 1);
    uint32_t offsetV = 0, offsetI = 0, lettersReady = 0;
    currentLine = 0, lettersInLine = 0;
    for (char& c : _text)
    {
        auto& character = atlas.Characters[c];

        // If letter doesn't fit into rect, it will be hidden
        bool isRendered = cursorX >= 0 && (float)cursorX + (float)character.Advance <= rectSize.x
                          && cursorY >= -atlas.MinY && (float)cursorY + (float)atlas.MaxY <= rectSize.y;
        if (isRendered)
        {
            glm::vec2 texCoords[4];
            texCoords[0] = glm::vec2(character.TopRight.x, character.BottomLeft.y);
            texCoords[1] = glm::vec2(character.TopRight.x, character.TopRight.y);
            texCoords[2] = glm::vec2(character.BottomLeft.x, character.BottomLeft.y);
            texCoords[3] = glm::vec2(character.BottomLeft.x, character.TopRight.y);

            // Calculate letter rect
            float x = ((float)cursorX + (float)character.Bearing.x + (float)character.Size.x * 0.5f) / rectSize.x - 0.5f;
            float y = ((float)cursorY + (float)character.Bearing.y - (float)character.Size.y * 0.5f) / rectSize.y - 0.5f;
            float hw = 0.5f * (float)character.Size.x / rectSize.x;
            float hh = 0.5f * (float)character.Size.y / rectSize.y;
            glm::vec3 letterVertices[4];
            letterVertices[0] = matrix * glm::vec4(x + hw, y + hh, 0.0f, 1.0f);
            letterVertices[1] = matrix * glm::vec4(x + hw, y - hh, 0.0f, 1.0f);
            letterVertices[2] = matrix * glm::vec4(x - hw, y + hh, 0.0f, 1.0f);
            letterVertices[3] = matrix * glm::vec4(x - hw, y - hh, 0.0f, 1.0f);

            for (int j = 0; j < 4; ++j)
            {
                vertices[offsetV++] = letterVertices[j][0];
                vertices[offsetV++] = letterVertices[j][1];
                vertices[offsetV++] = letterVertices[j][2];
                vertices[offsetV++] = _color[0];
                vertices[offsetV++] = _color[1];
                vertices[offsetV++] = _color[2];
                vertices[offsetV++] = _color[3];
                vertices[offsetV++] = texCoords[j][0];
                vertices[offsetV++] = texCoords[j][1];
            }

            indices[offsetI++] = lettersReady * 4 + 0;
            indices[offsetI++] = lettersReady * 4 + 1;
            indices[offsetI++] = lettersReady * 4 + 2;
            indices[offsetI++] = lettersReady * 4 + 1;
            indices[offsetI++] = lettersReady * 4 + 2;
            indices[offsetI++] = lettersReady * 4 + 3;

            lettersReady++;
        }

        // Save letter origins for text navigation needs
        lettersDimensions.emplace_back(cursorX, cursorY, character.Advance);
        lettersInLine++;
        cursorX += (int)character.Advance;

        if (lettersInLine >= lettersCount[currentLine] && currentLine < linesSize.size() - 1)
        {
            currentLine++;
            lettersInLine = 0;
            cursorX = OriginX(rectSize.x, linesSize[currentLine]);
            cursorY -= (int)spacing;
        }
    }

    // Add one more letter point in the end for new letters
    lettersDimensions.emplace_back(cursorX, cursorY, 0);

    std::vector<VertexAttribute> attributes;
    attributes.reserve(3);
    attributes.emplace_back(0, 3);
    attributes.emplace_back(1, 4);
    attributes.emplace_back(2, 2);

    vb.Create(vertices, verticesSize, attributes);
    ib.Create(indices, indicesSize);
}

bool UIText::IsNewLine(char c)
{
    return c == '\n';
}

bool UIText::IsSpace(char c)
{
    return !std::isalnum(c) && c != '-' && c != '\'' && c != '_';
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
                if ((float)linesSize[line] + character.Advance > maxWidth)
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
                if ((float)linesSize[line] + character.Advance > maxWidth)
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
            lastSpace = (int)lettersCount[line] - 1;
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
