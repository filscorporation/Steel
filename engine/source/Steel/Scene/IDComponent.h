#pragma once

#include "UUID.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/Entity.h"

class IDComponent : public Component
{
    COMPONENT(IDComponent)

public:
    explicit IDComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    static void RegisterType();

    UUID GetUUID() const;
    void SetUUID(UUID uuid);

private:
    UUID _uuid = 0;
};
