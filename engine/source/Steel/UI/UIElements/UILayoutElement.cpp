#include "UILayoutElement.h"

EntityID ILayoutElement::GetGroupID() const
{
    return _groupID;
}

void ILayoutElement::SetGroupID(EntityID groupID)
{
    _groupID = groupID;
}

void UILayoutElement::RegisterType()
{
    REGISTER_COMPONENT(UILayoutElement);
    REGISTER_ID_ATTRIBUTE(UILayoutElement, "groupID", GetGroupID, SetGroupID, AttributeFlags::Public);
}

LayoutElementInfo UILayoutElement::GetInfo()
{
    return Info;
}
