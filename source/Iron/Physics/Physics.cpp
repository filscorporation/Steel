#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Scene/Transformation.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Physics.h"
#include "PhysicsSystem.h"
#include "PhysicsCore.h"

PhysicsSystem* Physics::physicsSystem = nullptr;

void Physics::Init()
{
    PhysicsCore::CreateWorld();

    physicsSystem = new PhysicsSystem();
    Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->RegisterSystem<RigidBody>(physicsSystem);

    Log::LogInfo("Physics initialized");
}

void Physics::Terminate()
{
    delete physicsSystem;

    PhysicsCore::DeleteWorld();
}

void Physics::UpdatePhysicsTransformations()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto rigidBodies = registry->GetComponentIterator<RigidBody>();
    for (auto& rigidBody : rigidBodies)
    {
        rigidBody.UpdatePhysicsTransformation();
    }

    // TODO: use Collider type after components relations will be implemented
    auto boxColliders = registry->GetComponentIterator<BoxCollider>();
    for (auto& boxCollider : boxColliders)
    {
        boxCollider.SetSizeAutomatically();
    }

    auto circleColliders = registry->GetComponentIterator<CircleCollider>();
    for (auto& circleCollider : circleColliders)
    {
        circleCollider.SetSizeAutomatically();
    }
}

void Physics::Simulate(float deltaTime)
{
    PhysicsCore::Simulate(deltaTime);
}

std::vector<EntityID> Physics::PointCast(glm::vec2 center)
{
    return PhysicsCore::PointCast(center);
}

std::vector<EntityID> Physics::AABBCast(glm::vec2 center, glm::vec2 size)
{
    return PhysicsCore::AABBCast(center, size);
}

std::vector<RayCastHit> Physics::LineCast(glm::vec2 pointA, glm::vec2 pointB)
{
    return PhysicsCore::LineCast(pointA, pointB);
}
