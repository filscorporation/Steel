#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

class PropertiesView : public UIComponent
{
    DEFINE_COMPONENT(PropertiesView)

public:
    explicit PropertiesView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

    void DrawProperties(EntitiesRegistry* entitiesRegistry, std::vector<EntityID>& entities);
    void ClearProperties(EntitiesRegistry* entitiesRegistry);

private:
    EntityID CreateSimpleStringElement(EntitiesRegistry* entitiesRegistry, UILayer* layer, const std::string& text, const std::string& name, float& x, float& y);

    std::vector<EntityID> uiElementsEntities;
};
