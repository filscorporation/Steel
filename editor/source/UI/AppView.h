#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

class AppView : public UIComponent
{
    COMPONENT(AppView)

public:
    explicit AppView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

private:
    void UpdateView(EntitiesRegistry* entitiesRegistry) const;
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
