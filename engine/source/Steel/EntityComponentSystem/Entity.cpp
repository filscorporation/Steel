#include "Entity.h"

#define ENTITY_ID_MASK 0xFFFFFu
#define ENTITY_VERSION_MASK 0xFFFu
#define ENTITY_ID_SHIFT 12u

EntityID Entity::EntityIDGetID(EntityID entityID)
{
    return (entityID & ENTITY_ID_MASK << ENTITY_ID_SHIFT) >> ENTITY_ID_SHIFT;
}

EntityID Entity::EntityIDGetVersion(EntityID entityID)
{
    return entityID & ENTITY_VERSION_MASK;
}

EntityID Entity::EntityIDCombine(EntityID id, EntityID version)
{
    return (id << ENTITY_ID_SHIFT) + version;
}

EntityID Entity::EntityIDIncrementVersion(EntityID id)
{
    return EntityIDGetVersion(id) == ENTITY_VERSION_MASK
           ? EntityIDCombine(EntityIDGetID(id), 1)
           : id + 1;
}
