#pragma once

#include <glm/glm.hpp>
#include "Collider.h"

class CircleCollider : public Component
{
    struct CircleColliderInfo;

public:
    explicit CircleCollider(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void SetSizeAutomatically();
    float GetRadius();
    void SetRadius(float radius);

private:
    CircleColliderInfo* info = nullptr;
    float _radius = 0.0f;

    friend class RigidBody;
    friend class PhysicsSystem;
};
