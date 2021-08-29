#pragma once

#include "UILayoutElement.h"
#include "LayoutGroupTypes.h"
#include "../UIComponent.h"
#include "../UIEventHandler.h"
#include "../RectTransformation.h"
#include "../../EntityComponentSystem/EntitiesRegistry.h"

class UILayoutGroup : public UIComponent, public ILayoutElement
{
public:
    explicit UILayoutGroup(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(UIEventHandler& eventHandler);
    void Rebuild(UILayer* layer, RectTransformation& transformation);

    LayoutElementInfo GetInfo() override;

    void SetType(LayoutGroupTypes::LayoutGroupType type);
    EntityID AddElement(LayoutElementInfo info);
    EntityID AddElement(EntityID elementID);

    bool LayoutGroupDirty = false;

private:
    std::vector<EntityID> elements;
    LayoutGroupTypes::LayoutGroupType _type = LayoutGroupTypes::Horizontal;
};