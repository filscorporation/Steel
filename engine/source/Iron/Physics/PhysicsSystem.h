#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

class PhysicsSystem : public ComponentSystem<RigidBody>, public ComponentSystem<BoxCollider>, public ComponentSystem<CircleCollider>
{
public:
    void OnComponentAdded(EntityID entityID, RigidBody& component) override;
    void OnComponentRemoved(EntityID entityID, RigidBody& component) override;
    void OnEntityEnabled(EntityID entityID, RigidBody& component) override;
    void OnEntityDisabled(EntityID entityID, RigidBody& component) override;

    void OnComponentAdded(EntityID entityID, BoxCollider& component) override;
    void OnComponentRemoved(EntityID entityID, BoxCollider& component) override;
    void OnEntityEnabled(EntityID entityID, BoxCollider& component) override;
    void OnEntityDisabled(EntityID entityID, BoxCollider& component) override;

    void OnComponentAdded(EntityID entityID, CircleCollider& component) override;
    void OnComponentRemoved(EntityID entityID, CircleCollider& component) override;
    void OnEntityEnabled(EntityID entityID, CircleCollider& component) override;
    void OnEntityDisabled(EntityID entityID, CircleCollider& component) override;
};
