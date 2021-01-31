#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
    friend class RigidBody;

    struct BoxColliderInfo;

public:
    BoxCollider();
    ~BoxCollider() override;

private:
    BoxColliderInfo* info;
};
