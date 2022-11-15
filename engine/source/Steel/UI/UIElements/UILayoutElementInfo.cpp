#include "UILayoutElementInfo.h"
#include "Steel/Serialization/AttributesRegistration.h"

void UILayoutElementInfo::RegisterType()
{
    REGISTER_TYPE(UILayoutElementInfo);
    REGISTER_ID_ATTRIBUTE(UILayoutElementInfo, "elementID", GetElementID, SetElementID, AttributeFlags::Public, "Element ID");
    REGISTER_ATTRIBUTE(UILayoutElementInfo, "minWidth", GetMinWidth, SetMinWidth, float, AttributeFlags::Public, "Min width");
    REGISTER_ATTRIBUTE(UILayoutElementInfo, "minHeight", GetMinHeight, SetMinHeight, float, AttributeFlags::Public, "Min height");
    REGISTER_ATTRIBUTE(UILayoutElementInfo, "prefWidth", GetPreferredWidth, SetPreferredWidth, float, AttributeFlags::Public, "Pref width");
    REGISTER_ATTRIBUTE(UILayoutElementInfo, "prefHeight", GetPreferredHeight, SetPreferredHeight, float, AttributeFlags::Public, "Pref height");
}

EntityID UILayoutElementInfo::GetElementID() const
{
    return ElementID;
}

void UILayoutElementInfo::SetElementID(EntityID elementID)
{
    ElementID = elementID;
}

float UILayoutElementInfo::GetMinWidth() const
{
    return MinWidth;
}

void UILayoutElementInfo::SetMinWidth(float minWidth)
{
    MinWidth = minWidth;
}

float UILayoutElementInfo::GetMinHeight() const
{
    return MinHeight;
}

void UILayoutElementInfo::SetMinHeight(float minHeight)
{
    MinHeight = minHeight;
}

float UILayoutElementInfo::GetPreferredWidth() const
{
    return PreferredWidth;
}

void UILayoutElementInfo::SetPreferredWidth(float prefWidth)
{
    PreferredWidth = prefWidth;
}

float UILayoutElementInfo::GetPreferredHeight() const
{
    return PreferredHeight;
}

void UILayoutElementInfo::SetPreferredHeight(float prefHeight)
{
    PreferredHeight = prefHeight;
}
