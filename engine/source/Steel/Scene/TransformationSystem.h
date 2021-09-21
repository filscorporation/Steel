#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "../UI/RectTransformation.h"
#include "Transformation.h"

class TransformationSystem : public ComponentSystem<Transformation>, public ComponentSystem<RectTransformation>
{
public:
    void OnComponentAdded(EntityID entityID, Transformation& component) override;
    void OnComponentRemoved(EntityID entityID, Transformation& component) override;
    void OnEntityEnabled(EntityID entityID, Transformation& component) override;
    void OnEntityDisabled(EntityID entityID, Transformation& component) override;

    void OnComponentAdded(EntityID entityID, RectTransformation& component) override;
    void OnComponentRemoved(EntityID entityID, RectTransformation& component) override;
    void OnEntityEnabled(EntityID entityID, RectTransformation& component) override;
    void OnEntityDisabled(EntityID entityID, RectTransformation& component) override;
};
