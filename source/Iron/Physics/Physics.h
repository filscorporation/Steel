#pragma once

#include "../Scene/Entity.h"

class Physics
{
public:
    static void Init();
    static void Terminate();
    static void Simulate(float deltaTime);

    static std::vector<Entity*> PointCast(glm::vec2 center);
    static std::vector<Entity*> AABBCast(glm::vec2 center, glm::vec2 size);
};
