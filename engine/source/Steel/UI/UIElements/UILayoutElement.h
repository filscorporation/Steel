#pragma once

#include "Steel/UI/UIComponent.h"

struct LayoutElementInfo
{
    float MinWidth;
    float MinHeight;
    float PreferredWidth;
    float PreferredHeight;
};

class ILayoutElement
{
public:
    virtual LayoutElementInfo GetInfo() = 0;

    EntityID GetGroupID() const;
    void SetGroupID(EntityID groupID);

protected:
    EntityID _groupID = NULL_ENTITY;
};

class UILayoutElement : public UIComponent, public ILayoutElement
{
    DEFINE_COMPONENT(UILayoutElement)

public:
    explicit UILayoutElement(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    LayoutElementInfo GetInfo() override;

    LayoutElementInfo Info { };
};
