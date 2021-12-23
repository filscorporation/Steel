#include "SerializationContext.h"
#include "Steel/Scene/Scene.h"

UUID SerializationContext::GetUUID(EntityID entityID) const
{
    if (entityID == NULL_ENTITY)
        return NULL_UUID;
    auto registry = SerializedScene->GetEntitiesRegistry();
    return registry->GetComponent<IDComponent>(entityID).GetUUID();
}

EntityID SerializationContext::GetEntityID(UUID uuid) const
{
    return uuid == NULL_UUID ? NULL_ENTITY : SerializedScene->GetEntityByUUID(uuid);
}
