#pragma once

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/Entity.h"

class NameComponent : public Component
{
public:
    explicit NameComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    std::string Name;
    std::string Tag;
};
