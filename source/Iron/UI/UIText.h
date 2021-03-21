#pragma once

#include "UIComponent.h"

class UIText : public UIComponent
{
public:
    explicit UIText(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Rebuild();

    std::string GetText() const;
    void SetText(const std::string& text);
    uint32_t GetTextSize() const;
    void SetTextSize(uint32_t size);
    bool GetIsTextAutoSize() const;
    void SetIsTextAutoSize(bool isAutoSize);

private:
    std::string _text;
    uint32_t _textSize = 14;
    bool _isTextAutoSize = false;

    bool _dirtyText = false;
};
