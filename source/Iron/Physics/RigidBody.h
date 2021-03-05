#pragma once

#include "../Scene/Component.h"

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
    // TODO: change to physics core
    friend class Application;

    struct RigidBodyInfo;

public:
    explicit RigidBody(Entity* parentEntity);
    ~RigidBody() override;

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
    RigidBodyInfo* info;
    bool inGetTransformation = false;
    float _mass;
    RigidBodyTypes::RigidBodyType _type = RigidBodyTypes::None;

    void SetAutoFixture();
    void RemovePhysicsBody();
};
