#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

class CircleCollider : public Collider
{
    DEFINE_TYPE(CircleCollider)

    struct CircleColliderInfo;

public:
    explicit CircleCollider(EntityID ownerEntityID) : Collider(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void SetSizeAutomatically();
    float GetRadius() const;
    void SetRadius(float radius);

    bool IsSizeValid() const override;

private:
    void PrepareColliderInfo();
    void ApplyPhysicsProperties();

    CircleColliderInfo* info = nullptr;
    float _radius = 0.0f;
    bool autoSize = true;

    friend class RigidBody;
};
