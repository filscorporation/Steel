#pragma once

#include "RigidBody.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

#define SHAPE_EPS 0.00001f

struct RigidBody::RigidBodyInfo
{
    b2Body* Body;
    b2Fixture* Fixture;
};

struct BoxCollider::BoxColliderInfo
{
    b2PolygonShape* BoxShape;
};

struct CircleCollider::CircleColliderInfo
{
    b2CircleShape* CircleShape;
};
