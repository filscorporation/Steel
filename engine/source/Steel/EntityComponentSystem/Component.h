#pragma once

#include <cstdint>

#include "Entity.h"
#include "EntitiesRegistry.h"
#include "Steel/Serialization/Serializable.h"

class Component : public Serializable
{
public:
    explicit Component(EntityID ownerEntityID);
    virtual ~Component() = default;

    bool IsAlive() const;

    // Called when component is added to check conflicting or necessary components or data
    virtual bool Validate(EntitiesRegistry* entitiesRegistry) { return true; }
    // Called after component is added to set default values
    virtual void SetDefault(EntitiesRegistry* entitiesRegistry) { }

    virtual void OnCreated(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnRemoved(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnEnabled(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnDisabled(EntitiesRegistry* entitiesRegistry) { }

    EntityID Owner = NULL_ENTITY;
};
