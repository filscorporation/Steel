#pragma once

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Scene/UUID.h"

class Scene;

struct SerializationContext
{
public:
    Scene* SerializedScene = nullptr;

    UUID GetUUID(EntityID entityID) const;
    EntityID GetEntityID(UUID uuid) const;
};
