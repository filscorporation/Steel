#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

class BoxCollider : public Collider
{
    DEFINE_COMPONENT(BoxCollider)

    struct BoxColliderInfo;

public:
    explicit BoxCollider(EntityID ownerEntityID) : Collider(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void SetSizeAutomatically();
    const glm::vec2& GetSize() const;
    void SetSize(const glm::vec2& size);

    bool IsSizeValid() const override;

private:
    void PrepareColliderInfo();
    void ApplyPhysicsProperties();

    BoxColliderInfo* info = nullptr;
    glm::vec2 _size = glm::vec2(0.0f);
    bool autoSize = true;

    friend class RigidBody;
};
