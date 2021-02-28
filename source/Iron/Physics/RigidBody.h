#pragma once

#include "../Scene/Component.h"

class RigidBody : public Component
{
    // TODO: change to physics core
    friend class Application;

    struct RigidBodyInfo;

public:
    RigidBody();
    ~RigidBody() override;

    void SetDynamic();
    void SetStatic();
    void SetKinematic();

private:
    RigidBodyInfo* info;

    void SetAutoFixture();
    void GetPhysicsTransformation();
    void SetPhysicsTransformation();
};
