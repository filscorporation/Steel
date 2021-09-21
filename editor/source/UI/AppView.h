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

    void UpdateResolutionInfo(EntitiesRegistry* entitiesRegistry) const;
    EntityID tabEntity = NULL_ENTITY;
    EntityID statsEntity = NULL_ENTITY;
    EntityID appViewImageEntity = NULL_ENTITY;

    EntityID autoCBEntity = NULL_ENTITY;
    EntityID xFieldEntity = NULL_ENTITY;
    EntityID yFieldEntity = NULL_ENTITY;
    EntityID xTextEntity = NULL_ENTITY;
    EntityID yTextEntity = NULL_ENTITY;
};
