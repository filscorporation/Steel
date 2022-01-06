#include "UILayoutElement.h"

void UILayoutElement::RegisterType()
{
    REGISTER_TYPE(UILayoutElement);
    // TODO
}

LayoutElementInfo UILayoutElement::GetInfo()
{
    return Info;
}
