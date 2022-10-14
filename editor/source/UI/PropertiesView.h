#pragma once

#include "UIProperties/ComponentNodeWrapper.h"

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

#define ELEMENT_SIZE 18.0f
#define Y_OFFSET 4.0f
#define X_OFFSET 8.0f
#define BIG_OFFSET 10.0f

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

    bool isDirty = true;
    std::vector<EntityID> uiElementsEntities;
    std::vector<ComponentNodeWrapper*> componentsNodes;
    EntityID lastEntityID = NULL_ENTITY;
};
