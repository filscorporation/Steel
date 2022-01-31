#pragma once

#include "LayoutGroupTypes.h"
#include "UILayoutElementInfo.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/UI/RectTransformation.h"

class UILayoutGroup : public UIComponent
{
    DEFINE_COMPONENT(UILayoutGroup)

public:
    explicit UILayoutGroup(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(UILayer* layer, RectTransformation& transformation);

    void SetType(LayoutGroupTypes::LayoutGroupType type);
    LayoutGroupTypes::LayoutGroupType GetType() const;
    EntityID AddElement(float minWidth, float minHeight, float prefWidth, float prefHeight);
    EntityID AddElement(EntityID elementID);

private:
    const std::vector<UILayoutElementInfo>& GetElementsList() const;
    void SetElementsList(const std::vector<UILayoutElementInfo>& elements);

    UILayoutElementInfo GetInfo();

    std::vector<UILayoutElementInfo> _elements;
    LayoutGroupTypes::LayoutGroupType _type = LayoutGroupTypes::Horizontal;
};
