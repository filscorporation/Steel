#include <box2d/box2d.h>

#include "PhysicsInfo.h"
#include "PhysicsSystem.h"
#include "PhysicsCore.h"

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
    component.info = nullptr;
}

void PhysicsSystem::OnEntityEnabled(EntityID entityID, RigidBody& component)
{
    component.info = new RigidBody::RigidBodyInfo();
    auto typeBackup = component._type;
    component._type = RigidBodyTypes::None;
    component.SetType(typeBackup);
}

void PhysicsSystem::OnEntityDisabled(EntityID entityID, RigidBody& component)
{
    if (component.info == nullptr)
        return;

    PhysicsCore::GetWorld()->DestroyBody(component.info->Body);
    delete component.info;
    component.info = nullptr;
}

void PhysicsSystem::OnComponentAdded(EntityID entityID, BoxCollider& component)
{
    component.info = new BoxCollider::BoxColliderInfo();
    component.info->BoxShape = new b2PolygonShape();
    component.SetSizeAutomatically();
}

void PhysicsSystem::OnComponentRemoved(EntityID entityID, BoxCollider& component)
{
    if (component.info == nullptr)
        return;

    delete component.info;
}

void PhysicsSystem::OnEntityEnabled(EntityID entityID, BoxCollider& component)
{

}

void PhysicsSystem::OnEntityDisabled(EntityID entityID, BoxCollider& component)
{

}

void PhysicsSystem::OnComponentAdded(EntityID entityID, CircleCollider& component)
{
    component.info = new CircleCollider::CircleColliderInfo();
    component.info->CircleShape = new b2CircleShape();
    component.SetSizeAutomatically();
}

void PhysicsSystem::OnComponentRemoved(EntityID entityID, CircleCollider& component)
{
    if (component.info == nullptr)
        return;

    delete component.info;
}

void PhysicsSystem::OnEntityEnabled(EntityID entityID, CircleCollider& component)
{

}

void PhysicsSystem::OnEntityDisabled(EntityID entityID, CircleCollider& component)
{

}
