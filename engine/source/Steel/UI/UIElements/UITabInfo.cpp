#include "UITabInfo.h"
#include "Steel/Serialization/AttributesRegistration.h"

void UITabInfo::RegisterType()
{
    REGISTER_TYPE(UITabInfo);
    REGISTER_ID_ATTRIBUTE(UITabInfo, "button", GetButtonID, SetButtonID, AttributeFlags::Public, "Button");
    REGISTER_ID_ATTRIBUTE(UITabInfo, "content", GetContentID, SetContentID, AttributeFlags::Public, "Content");
}

UITabInfo::UITabInfo(EntityID buttonID, EntityID contentID)
{
    ButtonID = buttonID;
    ContentID = contentID;
}

EntityID UITabInfo::GetButtonID() const
{
    return ButtonID;
}

void UITabInfo::SetButtonID(EntityID buttonID)
{
    ButtonID = buttonID;
}

EntityID UITabInfo::GetContentID() const
{
    return ContentID;
}

void UITabInfo::SetContentID(EntityID contentID)
{
    ContentID = contentID;
}
