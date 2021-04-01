#pragma once

#include "../EntityComponentSystem/Component.h"

namespace RigidBodyTypes
{
    enum RigidBodyType
    {
        None,
        Dynamic,
        Static,
        Kinematic,
    };
}

// TODO: hide class physics logic in PhysicsCore
class RigidBody : public Component
{
    friend class PhysicsSystem;

    struct RigidBodyInfo;

public:
    explicit RigidBody(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void SetDynamic();
    void SetStatic();
    void SetKinematic();

    float GetMass() const;
    void SetMass(float mass);
    RigidBodyTypes::RigidBodyType GetType() const;
    void SetType(RigidBodyTypes::RigidBodyType type);

    void GetPhysicsTransformation();
    void UpdatePhysicsTransformation();

private:
    RigidBodyInfo* info = nullptr;
    bool inGetTransformation = false;
    float _mass = 1.0f;
    RigidBodyTypes::RigidBodyType _type = RigidBodyTypes::None;

    void SetAutoFixture();
};
