#include "PhysicsContactListener.h"
#include "RigidBody.h"
#include "../Scene/SceneHelper.h"

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
    auto entityA = (EntityID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (EntityID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    if (HasComponentS<RigidBody>(entityA))
    {
        auto& rbA = GetComponentS<RigidBody>(entityA);
        rbA.GetPhysicsTransformation();

        if (HasComponentS<ScriptComponent>(entityA))
        {
            auto& scriptA = GetComponentS<ScriptComponent>(entityA);
            scriptA.OnCollisionEnter(Collision { entityB });

            if (scriptA.ScriptsMask & ScriptEventTypes::OnCollisionStay)
                rbA.ActiveContacts[entityB] = Collision { entityB };
        }
    }
    if (HasComponentS<RigidBody>(entityB))
    {
        auto& rbB = GetComponentS<RigidBody>(entityB);
        rbB.GetPhysicsTransformation();

        if (HasComponentS<ScriptComponent>(entityB))
        {
            auto& scriptB = GetComponentS<ScriptComponent>(entityB);
            scriptB.OnCollisionEnter(Collision { entityA });

            if (scriptB.ScriptsMask & ScriptEventTypes::OnCollisionStay)
                rbB.ActiveContacts[entityA] = Collision { entityA };
        }
    }
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
    auto entityA = (EntityID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (EntityID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    if (HasComponentS<RigidBody>(entityA))
    {
        auto& rbA = GetComponentS<RigidBody>(entityA);
        rbA.GetPhysicsTransformation();
        rbA.ActiveContacts.erase(entityB);

        if (HasComponentS<ScriptComponent>(entityA))
        {
            auto& scriptA = GetComponentS<ScriptComponent>(entityA);
            scriptA.OnCollisionExit(Collision { entityB });
        }
    }
    if (HasComponentS<RigidBody>(entityB))
    {
        auto& rbB = GetComponentS<RigidBody>(entityB);
        rbB.GetPhysicsTransformation();
        rbB.ActiveContacts.erase(entityA);

        if (HasComponentS<ScriptComponent>(entityB))
        {
            auto& scriptB = GetComponentS<ScriptComponent>(entityB);
            scriptB.OnCollisionExit(Collision { entityA });
        }
    }
}
