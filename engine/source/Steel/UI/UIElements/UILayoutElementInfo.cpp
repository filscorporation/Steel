#include "UILayoutElementInfo.h"

void UILayoutElementInfo::RegisterType()
{
    REGISTER_TYPE(UILayoutElementInfo);
    REGISTER_ID_ATTRIBUTE(UILayoutElementInfo, "element", GetElementID, SetElementID, AttributeFlags::Public);
}

UILayoutElementInfo::UILayoutElementInfo(EntityID elementID)
{
    ElementID = elementID;
}

EntityID UILayoutElementInfo::GetElementID() const
{
    return ElementID;
}

void UILayoutElementInfo::SetElementID(EntityID elementID)
{
    ElementID = elementID;
}
