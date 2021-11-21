#pragma once

#include "HierarchyView.h"

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

class HierarchyElement : public UIComponent
{
    COMPONENT(HierarchyElement)

public:
    explicit HierarchyElement(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, UILayer* layer,
            const HierarchyView& hierarchyView, EntityID nodeEntity, const HierarchyViewNode& node);
    void UpdatePosition(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, EntityID nodeEntity, const HierarchyViewNode& node);

    void SetDefaultNodeStyle(EntitiesRegistry* entitiesRegistry) const;
    void SetSelectedNodeStyle(EntitiesRegistry* entitiesRegistry) const;
private:

    EntityID buttonEntityID = NULL_ENTITY;
    void SetPosition(EntitiesRegistry* entitiesRegistry, int order);
};
