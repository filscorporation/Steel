#pragma once

#include <cstdint>

#include "Entity.h"
#include "EntitiesRegistry.h"
#include "../Serialization/Serializable.h"

class Component : public Serializable
{
public:
    explicit Component(EntityID ownerEntityID);
    virtual ~Component() = default;

    virtual void OnCopied() { }

    bool IsAlive() const;

    virtual bool Validate(EntitiesRegistry* entitiesRegistry) { return true; }
    virtual void OnCreated(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnRemoved(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnEnabled(EntitiesRegistry* entitiesRegistry) { }
    virtual void OnDisabled(EntitiesRegistry* entitiesRegistry) { }

    EntityID Owner = NULL_ENTITY;
};
