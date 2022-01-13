#pragma once

#include <glm/glm.hpp>

#include "Collider.h"

class BoxCollider : public Component
{
    COMPONENT(BoxCollider)

    struct BoxColliderInfo;

public:
    explicit BoxCollider(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void ApplyPhysicsProperties();

    void SetSizeAutomatically();
    const glm::vec2& GetSize() const;
    void SetSize(const glm::vec2& size);

private:
    BoxColliderInfo* info = nullptr;
    glm::vec2 _size = glm::vec2(0.0f);
    bool autoSize = true;

    friend class RigidBody;
};
