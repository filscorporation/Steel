#pragma once

#include <glm/glm.hpp>
#include "Collider.h"

class BoxCollider : public Collider
{
    friend class RigidBody;

    struct BoxColliderInfo;

public:
    BoxCollider();
    ~BoxCollider() override;

    void SetSizeAutomatically();
    glm::vec2 GetSize();
    void SetSize(glm::vec2 size);

private:
    BoxColliderInfo* info;
    glm::vec2 _size;
};
