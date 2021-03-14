#pragma once

#include <glm/glm.hpp>
#include "Collider.h"

class CircleCollider : public Collider
{
    friend class RigidBody;

    struct CircleColliderInfo;

public:
    explicit CircleCollider(EntityID ownerEntityID);
    ~CircleCollider() override;

    void SetSizeAutomatically();
    float GetRadius();
    void SetRadius(float radius);

private:
    CircleColliderInfo* info;
    float _radius;
};
