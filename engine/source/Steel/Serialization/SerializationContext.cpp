#include "SerializationContext.h"
#include "Steel/Scene/Scene.h"
#include "Steel/Resources/ResourcesManager.h"

UUID_TYPE SerializationContext::GetUUID(EntityID entityID) const
{
    if (entityID == NULL_ENTITY)
        return NULL_UUID;
    auto registry = SerializedScene->GetEntitiesRegistry();
    return registry->HasComponent<IDComponent>(entityID)
            ? registry->GetComponent<IDComponent>(entityID).GetUUID()
            : NULL_UUID;
}

EntityID SerializationContext::GetEntityID(UUID_TYPE uuid) const
{
    return uuid == NULL_UUID ? NULL_ENTITY : SerializedScene->GetEntityByUUID(uuid);
}

ResourceID SerializationContext::GetResourceID(Resource* resource) const
{
    return resource == nullptr ? NULL_RESOURCE : resource->ID;
}

Resource* SerializationContext::GetResource(ResourceTypes::ResourceType resourceType, ResourceID resourceID) const
{
    return ResourcesSource->GetResource(resourceType, resourceID);
}
