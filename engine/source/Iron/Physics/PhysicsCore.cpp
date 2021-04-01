#include <box2d/box2d.h>

#include "PhysicsCore.h"
#include "../Core/Log.h"

#define POINT_AABB_SIZE 0.001f

const float GRAVITY = -9.8f;
const int VELOCITY_ITERATIONS = 6;
const int POSITION_ITERATIONS = 2;

static b2World* world;
static PhysicsContactListener contactListener;

class QueryCallback : public b2QueryCallback
{
public:
    std::vector<b2Body*> FoundBodies;
    bool IsPoint = false;
    glm::vec2 TestPoint;

    bool ReportFixture(b2Fixture* fixture) override
    {
        if (!IsPoint || fixture->TestPoint(b2Vec2(TestPoint.x, TestPoint.y)))
            FoundBodies.push_back(fixture->GetBody());
        return true;
    }
};
static QueryCallback queryCallback;

class RayCastCallback : public b2RayCastCallback
{
public:
    std::vector<RayCastHit> RayCastHits;

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                        const b2Vec2& normal, float fraction) override
    {
        RayCastHit hit{};
        hit.BodyEntity = (EntityID)fixture->GetBody()->GetUserData().pointer;
        hit.HitPoint = glm::vec2(point.x, point.y);
        hit.HitNormal = glm::vec2(normal.x, normal.y);

        RayCastHits.push_back(hit);

        return true;
    }
};
static RayCastCallback rayCastCallback;

void PhysicsCore::CreateWorld()
{
    b2Vec2 gravity(0.0f, GRAVITY);
    world = new b2World(gravity);
    world->SetContactListener(&contactListener);
}

void PhysicsCore::DeleteWorld()
{
    delete world;
}

void PhysicsCore::Simulate(float deltaTime)
{
    world->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

b2World *PhysicsCore::GetWorld()
{
    return world;
}

std::vector<EntityID> PhysicsCore::PointCast(glm::vec2 center)
{
    auto aabb = b2AABB();
    aabb.lowerBound.Set(center.x - POINT_AABB_SIZE * 0.5f, center.y - POINT_AABB_SIZE * 0.5f);
    aabb.upperBound.Set(center.x + POINT_AABB_SIZE * 0.5f, center.y + POINT_AABB_SIZE * 0.5f);

    queryCallback.IsPoint = true;
    queryCallback.TestPoint = center;
    world->QueryAABB(&queryCallback, aabb);

    std::vector<EntityID> result;
    for (auto body : queryCallback.FoundBodies)
    {
        result.push_back((EntityID)body->GetUserData().pointer);
    }

    queryCallback.FoundBodies.clear();

    return result;
}

std::vector<EntityID> PhysicsCore::AABBCast(glm::vec2 center, glm::vec2 size)
{
    // TODO: gives imprecise results
    auto aabb = b2AABB();
    aabb.lowerBound.Set(center.x - size.x * 0.5f, center.y - size.y * 0.5f);
    aabb.upperBound.Set(center.x + size.x * 0.5f, center.y + size.y * 0.5f);

    queryCallback.IsPoint = false;
    world->QueryAABB(&queryCallback, aabb);

    std::vector<EntityID> result;
    for (auto body : queryCallback.FoundBodies)
    {
        result.push_back((EntityID)body->GetUserData().pointer);
    }

    queryCallback.FoundBodies.clear();

    return result;
}

std::vector<RayCastHit> PhysicsCore::LineCast(glm::vec2 pointA, glm::vec2 pointB)
{
    rayCastCallback.RayCastHits.clear();
    world->RayCast(&rayCastCallback, b2Vec2(pointA.x, pointA.y), b2Vec2(pointB.x, pointB.y));

    return rayCastCallback.RayCastHits;
}
