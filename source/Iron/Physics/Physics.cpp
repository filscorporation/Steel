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
    auto transforms = Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->GetComponentIterator<Transformation>();
    for (auto& transform : transforms)
    {
        if (transform.DidTransformationChange())
        {
            auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
            if (registry->HasComponent<RigidBody>(transform.Owner))
            {
                auto& rb = registry->GetComponent<RigidBody>(transform.Owner);
                rb.UpdatePhysicsTransformation();
            }
            // TODO: use Collider type after components relations will be implemented
            if (registry->HasComponent<BoxCollider>(transform.Owner))
            {
                auto& bc = registry->GetComponent<BoxCollider>(transform.Owner);
                bc.SetSizeAutomatically();
            }
            if (registry->HasComponent<CircleCollider>(transform.Owner))
            {
                auto& cc = registry->GetComponent<CircleCollider>(transform.Owner);
                cc.SetSizeAutomatically();
            }
        }
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
