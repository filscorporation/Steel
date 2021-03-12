#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "RigidBody.h"

class PhysicsSystem : public ComponentSystem<RigidBody>
{
public:
    void OnComponentAdded(EntityID entityID, RigidBody& component) override;
    void OnComponentRemoved(EntityID entityID, RigidBody& component) override;
};
