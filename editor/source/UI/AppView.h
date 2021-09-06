#pragma once

#include <Iron/UI/UIComponent.h>
#include <Iron/EntityComponentSystem/EntitiesRegistry.h>

class AppView : public UIComponent
{
public:
    explicit AppView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

private:
    EntityID appViewImageEntity = NULL_ENTITY;
};
