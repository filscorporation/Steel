#pragma once

#include "AttributeInfo.h"
#include "AttributeAccessor.h"
#include "IDAttributeAccessor.h"
#include "EnumAttributeAccessor.h"
#include "ListAttributeAccessor.h"
#include "ResourceAttributeAccessor.h"
#include "SerializationManager.h"
#include "Steel/EntityComponentSystem/TypeInfoStorage.h"
#include "Steel/EntityComponentSystem/TypeInfo.h"

class Serializable
{

};

#define REGISTER_ATTRIBUTE(className, name, getter, setter, type, flags)                                        \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new AttributeAccessor<className, type, AttributeTypedef<type>>(&className::getter, &className::setter), \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_ENUM_ATTRIBUTE(className, name, getter, setter, type, flags)                                   \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new EnumAttributeAccessor<className, type>(&className::getter, &className::setter),                     \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_ID_ATTRIBUTE(className, name, getter, setter, flags)                                           \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new IDAttributeAccessor<className>(&className::getter, &className::setter),                             \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_RESOURCE_ATTRIBUTE(className, name, getter, setter, type, resourceType, flags)                 \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new ResourceAttributeAccessor<className, type>(&className::getter, &className::setter, resourceType),   \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_LIST_ATTRIBUTE(className, name, getter, setter, type, flags)                                       \
SerializationManager::RegisterAttribute                                                                             \
(                                                                                                                   \
    TYPE_ID(className),                                                                                             \
    AttributeInfo                                                                                                   \
    (                                                                                                               \
        name,                                                                                                       \
        new ListAttributeAccessor<className, type>(&className::getter, &className::setter),                         \
        flags                                                                                                       \
    )                                                                                                               \
)
