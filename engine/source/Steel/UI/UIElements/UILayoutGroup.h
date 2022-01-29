#pragma once

#include "UILayoutElement.h"
#include "LayoutGroupTypes.h"
#include "UILayoutElementInfo.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/UI/RectTransformation.h"

class UILayoutGroup : public UIComponent, public ILayoutElement
{
    DEFINE_COMPONENT(UILayoutGroup)

public:
    explicit UILayoutGroup(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(UILayer* layer, RectTransformation& transformation);

    LayoutElementInfo GetInfo() override;

    void SetType(LayoutGroupTypes::LayoutGroupType type);
    LayoutGroupTypes::LayoutGroupType GetType() const;
    EntityID AddElement(LayoutElementInfo info);
    EntityID AddElement(EntityID elementID);

    bool LayoutGroupDirty = false;

private:
    const std::vector<UILayoutElementInfo>& GetElementsList() const;
    void SetElementsList(const std::vector<UILayoutElementInfo>& elements);

    std::vector<UILayoutElementInfo> _elements;
    LayoutGroupTypes::LayoutGroupType _type = LayoutGroupTypes::Horizontal;
};
