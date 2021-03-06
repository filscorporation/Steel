#include "Physics.h"
#include "PhysicsCore.h"
#include "../Core/Log.h"

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

std::vector<Entity *> Physics::PointCast(glm::vec2 center)
{
    return PhysicsCore::PointCast(center);
}

std::vector<Entity*> Physics::AABBCast(glm::vec2 center, glm::vec2 size)
{
    return PhysicsCore::AABBCast(center, size);
}
