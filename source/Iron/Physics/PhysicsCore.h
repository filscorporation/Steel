#pragma once

#include <box2d/box2d.h>
#include <glm/vec2.hpp>
#include <vector>

#include "PhysicsContactListener.h"
#include "../EntityComponentSystem/ComponentSystem.h"
#include "RigidBody.h"

class PhysicsCore
{
public:
    static void CreateWorld();
    static b2World* GetWorld();
    static void DeleteWorld();

    static std::vector<EntityID> PointCast(glm::vec2 center);
    static std::vector<EntityID> AABBCast(glm::vec2 center, glm::vec2 size);

    static void Simulate(float deltaTime);
};
