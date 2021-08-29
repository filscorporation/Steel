#pragma once

#include "../UIComponent.h"

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

    EntityID GroupID = NULL_ENTITY;
};

class UILayoutElement : public UIComponent, public ILayoutElement
{
public:
    explicit UILayoutElement(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    LayoutElementInfo GetInfo() override { return Info; };

    LayoutElementInfo Info { };
};
