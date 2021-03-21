#include "UIText.h"

void UIText::Rebuild()
{
    if (!_dirtyText)
        return;

    _dirtyText = false;

    // TODO: build text data to render and put into UIRenderer
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

    _textSize = size;
    _dirtyText = true;
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
