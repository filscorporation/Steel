#pragma once

#include <glm/glm.hpp>
#include "Collider.h"

class CircleCollider : public Component
{
    struct CircleColliderInfo;

public:
    explicit CircleCollider(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init();

    void SetSizeAutomatically();
    float GetRadius() const;
    void SetRadius(float radius);

private:
    CircleColliderInfo* info = nullptr;
    float _radius = 0.0f;
    bool autoSize = true;

    friend class RigidBody;
};
