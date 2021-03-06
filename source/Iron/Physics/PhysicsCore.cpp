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

std::vector<Entity*> PhysicsCore::PointCast(glm::vec2 center)
{
    auto aabb = b2AABB();
    aabb.lowerBound.Set(center.x - POINT_AABB_SIZE * 0.5f, center.y - POINT_AABB_SIZE * 0.5f);
    aabb.upperBound.Set(center.x + POINT_AABB_SIZE * 0.5f, center.y + POINT_AABB_SIZE * 0.5f);

    queryCallback.IsPoint = true;
    queryCallback.TestPoint = center;
    world->QueryAABB(&queryCallback, aabb);

    std::vector<Entity*> result;
    for (auto body : queryCallback.FoundBodies)
    {
        result.push_back((Entity*)body->GetUserData().pointer);
    }

    queryCallback.FoundBodies.clear();

    return result;
}

std::vector<Entity*> PhysicsCore::AABBCast(glm::vec2 center, glm::vec2 size)
{
    // TODO: gives imprecise results
    auto aabb = b2AABB();
    aabb.lowerBound.Set(center.x - size.x * 0.5f, center.y - size.y * 0.5f);
    aabb.upperBound.Set(center.x + size.x * 0.5f, center.y + size.y * 0.5f);

    queryCallback.IsPoint = false;
    world->QueryAABB(&queryCallback, aabb);

    std::vector<Entity*> result;
    for (auto body : queryCallback.FoundBodies)
    {
        result.push_back((Entity*)body->GetUserData().pointer);
    }

    queryCallback.FoundBodies.clear();

    return result;
}
