#include <box2d/box2d.h>

#include "PhysicsCore.h"
#include "../Core/Log.h"

const int VELOCITY_ITERATIONS = 6;
const int POSITION_ITERATIONS = 2;

static b2World* World;
static PhysicsContactListener contactListener;

void PhysicsCore::Init()
{
    b2Vec2 gravity(0.0f, -10.0f);
    World = new b2World(gravity);
    World->SetContactListener(&contactListener);

    Log::LogInfo("Physics initialized");
}

void PhysicsCore::Terminate()
{
    delete World;
}

void PhysicsCore::Simulate(float deltaTime)
{
    World->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

b2World *PhysicsCore::GetWorld()
{
    return World;
}
