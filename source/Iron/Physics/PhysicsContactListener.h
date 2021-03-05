#pragma once

#include <box2d/box2d.h>

#include "../Scene/Entity.h"
#include "../Scripting/ScriptComponent.h"

class PhysicsContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;

    void EndContact(b2Contact* contact) override;

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
    {
        B2_NOT_USED(contact);
        B2_NOT_USED(oldManifold);
    }

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
    {
        B2_NOT_USED(contact);
        B2_NOT_USED(impulse);
    }
};
