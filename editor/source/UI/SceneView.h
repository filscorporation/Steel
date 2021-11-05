#pragma once

#include "ControlPanel.h"

#include <glm/glm.hpp>

class SceneView : public UIComponent
{
public:
    explicit SceneView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

    void FocusCameraOnEntity(EntitiesRegistry* entitiesRegistry, const std::vector<EntityID>& selectedEntities);

private:
    void UpdateView(EntitiesRegistry* entitiesRegistry);

    void UpdateCameraDrag(EntitiesRegistry* entitiesRegistry);
    void UpdateCameraScroll(EntitiesRegistry* entitiesRegistry);

    EntityID sceneViewImageEntity = NULL_ENTITY;
    bool ignoreAppInput = false;
    bool isDirty = false;
};
