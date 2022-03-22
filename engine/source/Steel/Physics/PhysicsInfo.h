#pragma once

#include "RigidBody.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

#define SHAPE_EPS 0.001f

struct RigidBody::RigidBodyInfo
{
    b2Body* Body;
    b2Fixture* Fixture;
    bool IsFixtureValid = false;
};

struct BoxCollider::BoxColliderInfo
{
    b2PolygonShape* BoxShape;
};

struct CircleCollider::CircleColliderInfo
{
    b2CircleShape* CircleShape;
};
