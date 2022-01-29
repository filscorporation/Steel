#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

class CircleCollider : public Component
{
    DEFINE_COMPONENT(CircleCollider)

    struct CircleColliderInfo;

public:
    explicit CircleCollider(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void SetSizeAutomatically();
    float GetRadius() const;
    void SetRadius(float radius);

private:
    void PrepareColliderInfo();
    void ApplyPhysicsProperties();

    CircleColliderInfo* info = nullptr;
    float _radius = 0.0f;
    bool autoSize = true;

    friend class RigidBody;
};
