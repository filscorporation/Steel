#include "../Core/Log.h"
#include "Physics.h"
#include "PhysicsCore.h"

void Physics::Init()
{
    PhysicsCore::CreateWorld();

    Log::LogInfo("Physics initialized");
}

void Physics::Terminate()
{
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
