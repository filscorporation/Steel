#pragma once

#include <Steel/UI/UIComponent.h>

class SceneView : public UIComponent
{
public:
    explicit SceneView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

private:
    void UpdateView(EntitiesRegistry* entitiesRegistry) const;

    EntityID sceneViewImageEntity = NULL_ENTITY;
    bool ignoreInput = false;
};
