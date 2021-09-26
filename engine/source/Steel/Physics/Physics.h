#pragma once

#include <vector>
#include <glm/vec2.hpp>

#include "../EntityComponentSystem/Entity.h"
#include "Collision.h"

struct RayCastHit
{
    EntityID BodyEntity;
    glm::vec2 HitPoint;
    glm::vec2 HitNormal;
};

class Physics
{
public:
    static void CreatePhysicsScene(EntitiesRegistry* entitiesRegistry);
    static void DeletePhysicsScene();
    static void UpdatePhysicsTransformations();
    static void Simulate(float deltaTime);
    static void GetPhysicsTransformations();
    static void SendEvents();

    static std::vector<EntityID> PointCast(glm::vec2 center);
    static std::vector<EntityID> AABBCast(glm::vec2 center, glm::vec2 size);
    static std::vector<RayCastHit> LineCast(glm::vec2 pointA, glm::vec2 pointB);

private:
    static EntityID lastMouseOverCollider;
};
