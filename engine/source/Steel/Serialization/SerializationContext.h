#pragma once

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Scene/UUID.h"
#include "Steel/Resources/Resource.h"

class Scene;
class ResourcesManager;

struct SerializationContext
{
public:
    Scene* SerializedScene = nullptr;
    ResourcesManager* ResourcesSource = nullptr;

    UUID_TYPE GetUUID(EntityID entityID) const;
    EntityID GetEntityID(UUID_TYPE uuid) const;

    ResourceID GetResourceID(Resource* resource) const;
    Resource* GetResource(ResourceTypes::ResourceType resourceType, ResourceID resourceID) const;
};
