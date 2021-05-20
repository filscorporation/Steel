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
    Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->RegisterSystem<BoxCollider>(physicsSystem);
    Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->RegisterSystem<CircleCollider>(physicsSystem);

    Log::LogDebug("Physics initialized");
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
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
    {
        if (rigidBodies[i].IsAlive())
            rigidBodies[i].UpdatePhysicsTransformation();
    }

    // TODO: use Collider type after components relations will be implemented
    auto boxColliders = registry->GetComponentIterator<BoxCollider>();
    int boxCollidersSize = boxColliders.Size();
    for (int i = 0; i < boxCollidersSize; ++i)
    {
        if (boxColliders[i].IsAlive())
            boxColliders[i].SetSizeAutomatically();
    }

    auto circleColliders = registry->GetComponentIterator<CircleCollider>();
    int circleCollidersSize = circleColliders.Size();
    for (int i = 0; i < circleCollidersSize; ++i)
    {
        if (circleColliders[i].IsAlive())
            circleColliders[i].SetSizeAutomatically();
    }
}

void Physics::Simulate(float deltaTime)
{
    PhysicsCore::Simulate(deltaTime);
}

void Physics::GetPhysicsTransformations()
{
    auto rigidBodies = Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->GetComponentIterator<RigidBody>();
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
        if (rigidBodies[i].IsAlive())
            rigidBodies[i].GetPhysicsTransformation();
}

void Physics::SendEvents()
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto rigidBodies = entitiesRegistry->GetComponentIterator<RigidBody>();
    auto scriptsAccessor = entitiesRegistry->GetComponentAccessor<ScriptComponent>();
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
    {
        if (rigidBodies[i].IsAlive() && scriptsAccessor.Has(rigidBodies[i].Owner))
        {
            for (auto contact : rigidBodies[i].ActiveContacts)
            {
                scriptsAccessor.Get(rigidBodies[i].Owner).OnCollisionStay(contact.second);
            }
        }
    }
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
