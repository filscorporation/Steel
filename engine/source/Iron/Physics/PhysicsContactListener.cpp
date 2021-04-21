#include "PhysicsContactListener.h"
#include "RigidBody.h"
#include "../Core/Application.h"

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
    auto entityA = (EntityID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto entityB = (EntityID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (entitiesRegistry->HasComponent<RigidBody>(entityA))
    {
        auto& rbA = entitiesRegistry->GetComponent<RigidBody>(entityA);
        rbA.GetPhysicsTransformation();

        if (entitiesRegistry->HasComponent<ScriptComponent>(entityA))
        {
            auto& scriptA = entitiesRegistry->GetComponent<ScriptComponent>(entityA);
            scriptA.OnCollisionEnter(Collision { entityB });

            if (scriptA.ScriptsMask & ScriptEventTypes::OnCollisionStay)
                rbA.ActiveContacts[entityB] = Collision { entityB };
        }
    }
    if (entitiesRegistry->HasComponent<RigidBody>(entityB))
    {
        auto& rbB = entitiesRegistry->GetComponent<RigidBody>(entityB);
        rbB.GetPhysicsTransformation();

        if (entitiesRegistry->HasComponent<ScriptComponent>(entityB))
        {
            auto& scriptB = entitiesRegistry->GetComponent<ScriptComponent>(entityB);
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
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (entitiesRegistry->HasComponent<RigidBody>(entityA))
    {
        auto& rbA = entitiesRegistry->GetComponent<RigidBody>(entityA);
        rbA.GetPhysicsTransformation();
        rbA.ActiveContacts.erase(entityB);

        if (entitiesRegistry->HasComponent<ScriptComponent>(entityA))
        {
            auto& scriptA = entitiesRegistry->GetComponent<ScriptComponent>(entityA);
            scriptA.OnCollisionExit(Collision { entityB });
        }
    }
    if (entitiesRegistry->HasComponent<RigidBody>(entityB))
    {
        auto& rbB = entitiesRegistry->GetComponent<RigidBody>(entityB);
        rbB.GetPhysicsTransformation();
        rbB.ActiveContacts.erase(entityA);

        if (entitiesRegistry->HasComponent<ScriptComponent>(entityB))
        {
            auto& scriptB = entitiesRegistry->GetComponent<ScriptComponent>(entityB);
            scriptB.OnCollisionExit(Collision { entityA });
        }
    }
}
