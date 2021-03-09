#include "PhysicsContactListener.h"
#include "RigidBody.h"
#include "../Scene/SceneHelper.h"

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
    auto entityA = (EntityID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (EntityID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    GetComponentS<RigidBody>(entityA).GetPhysicsTransformation();
    GetComponentS<RigidBody>(entityB).GetPhysicsTransformation();

    if (HasComponentS<ScriptComponent>(entityA))
    {
        auto& scriptA = GetComponentS<ScriptComponent>(entityA);
        scriptA.OnCollisionEnter(Collision{ entityB });
    }
    if (HasComponentS<ScriptComponent>(entityB))
    {
        auto& scriptB = GetComponentS<ScriptComponent>(entityB);
        scriptB.OnCollisionEnter(Collision{ entityA });
    }
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
    auto entityA = (EntityID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (EntityID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    GetComponentS<RigidBody>(entityA).GetPhysicsTransformation();
    GetComponentS<RigidBody>(entityB).GetPhysicsTransformation();

    if (HasComponentS<ScriptComponent>(entityA))
    {
        auto& scriptA = GetComponentS<ScriptComponent>(entityA);
        scriptA.OnCollisionExit(Collision{ entityB });
    }
    if (HasComponentS<ScriptComponent>(entityB))
    {
        auto& scriptB = GetComponentS<ScriptComponent>(entityB);
        scriptB.OnCollisionExit(Collision{ entityA });
    }
}
