#include "PhysicsContactListener.h"
#include "RigidBody.h"

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
    auto entityA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    entityA->GetComponent<RigidBody>()->GetPhysicsTransformation();
    entityB->GetComponent<RigidBody>()->GetPhysicsTransformation();

    auto scriptA = entityA->GetComponent<ScriptComponent>();
    if (scriptA != nullptr)
    {
        scriptA->OnCollisionEnter(Collision{ entityB });
    }
    auto scriptB = entityB->GetComponent<ScriptComponent>();
    if (scriptB != nullptr)
    {
        scriptB->OnCollisionEnter(Collision{ entityA });
    }
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
    auto entityA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    entityA->GetComponent<RigidBody>()->GetPhysicsTransformation();
    entityB->GetComponent<RigidBody>()->GetPhysicsTransformation();

    auto scriptA = entityA->GetComponent<ScriptComponent>();
    if (scriptA != nullptr)
    {
        scriptA->OnCollisionExit(Collision{ entityB });
    }
    auto scriptB = entityB->GetComponent<ScriptComponent>();
    if (scriptB != nullptr)
    {
        scriptB->OnCollisionExit(Collision{ entityA });
    }
}