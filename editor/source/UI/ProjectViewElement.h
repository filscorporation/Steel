#pragma once

#include "ProjectView.h"

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

class ProjectViewElement : public UIComponent
{
    DEFINE_COMPONENT(ProjectViewElement)

public:
    explicit ProjectViewElement(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry, UILayer* layer, const ProjectView& projectView, const ProjectViewNode& node);

    void SetSelectedNodeStyle(EntitiesRegistry* entitiesRegistry) const;
    void SetDefaultNodeStyle(EntitiesRegistry* entitiesRegistry) const;

private:
    EntityID buttonEntityID = NULL_ENTITY;

    void SetPosition(EntitiesRegistry* entitiesRegistry, int order);
};
