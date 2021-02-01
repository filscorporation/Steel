#pragma once

#include "RigidBody.h"
#include "BoxCollider.h"

struct RigidBody::RigidBodyInfo
{
    b2Body* Body;
};

struct BoxCollider::BoxColliderInfo
{
    b2PolygonShape* GroundBox;
};
