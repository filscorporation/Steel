#pragma once

#include <box2d/box2d.h>

#include "PhysicsContactListener.h"

class PhysicsCore
{
public:
    static void CreateWorld();
    static b2World* GetWorld();
    static void DeleteWorld();

    static std::vector<Entity*> PointCast(glm::vec2 center);
    static std::vector<Entity*> AABBCast(glm::vec2 center, glm::vec2 size);

    static void Simulate(float deltaTime);
};
