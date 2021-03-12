#include "../Core/Log.h"
#include "Physics.h"
#include "PhysicsSystem.h"
#include "PhysicsCore.h"
#include "../Core/Application.h"

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
