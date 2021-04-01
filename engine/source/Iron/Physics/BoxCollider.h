#pragma once

#include <glm/glm.hpp>
#include "Collider.h"

class BoxCollider : public Component
{
    struct BoxColliderInfo;

public:
    explicit BoxCollider(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void SetSizeAutomatically();
    glm::vec2 GetSize();
    void SetSize(glm::vec2 size);

private:
    BoxColliderInfo* info = nullptr;
    glm::vec2 _size = glm::vec2(0.0f);

    friend class RigidBody;
    friend class PhysicsSystem;
};
