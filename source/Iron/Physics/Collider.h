#pragma once

#include "../Scene/Component.h"

class Collider : public Component
{
public:
    explicit Collider(Entity* parentEntity) : Component(parentEntity) { }
};
