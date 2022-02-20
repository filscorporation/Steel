#pragma once

#include "UUID.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/Entity.h"

class IDComponent : public Component
{
    DEFINE_COMPONENT(IDComponent)

public:
    explicit IDComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    UUID_TYPE GetUUID() const;
    void SetUUID(UUID_TYPE uuid);

private:
    UUID_TYPE _uuid = 0;
};
