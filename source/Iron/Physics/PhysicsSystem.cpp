#include <box2d/box2d.h>

#include "PhysicsInfo.h"
#include "PhysicsSystem.h"
#include "PhysicsCore.h"
#include "../Core/Log.h"

void PhysicsSystem::OnComponentAdded(EntityID entityID, RigidBody& component)
{
    component.info = new RigidBody::RigidBodyInfo();
}

void PhysicsSystem::OnComponentRemoved(EntityID entityID, RigidBody& component)
{
    if (component.info == nullptr)
        return;

    PhysicsCore::GetWorld()->DestroyBody(component.info->Body);
    delete component.info;
}
