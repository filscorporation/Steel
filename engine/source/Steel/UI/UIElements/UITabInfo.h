#pragma once

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Serialization/Serializable.h"
#include "Steel/EntityComponentSystem/TypeInfo.h"

struct UITabInfo : public Serializable
{
    DEFINE_TYPE(UITabInfo)

    UITabInfo() = default;
    UITabInfo(EntityID buttonID, EntityID contentID);

    EntityID GetButtonID() const;
    void SetButtonID(EntityID buttonID);
    EntityID GetContentID() const;
    void SetContentID(EntityID contentID);

    EntityID ButtonID = NULL_ENTITY;
    EntityID ContentID = NULL_ENTITY;
};
