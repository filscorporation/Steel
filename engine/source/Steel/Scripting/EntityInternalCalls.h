#pragma once

#include <cstdint>

#include "ScriptingCore.h"
#include "ScriptComponent.h"
#include "Steel/Scene/IDComponent.h"

namespace EntityInternalCalls
{
    EntityID Entity_CreateNewEntity();
    EntityID Entity_CreateNewEntity2(MonoString* name);
    EntityID Entity_CreateNewEntity3(MonoString* name, EntityID parentEntityID);
    bool Entity_DestroyEntity(EntityID id);
    MonoObject* Entity_AddComponent(EntityID id, void* type);
    MonoObject* Entity_GetComponent(EntityID id, void* type);
    bool Entity_HasComponent(EntityID id, void* type);
    bool Entity_RemoveComponent(EntityID id, void* type);

    bool Entity_GetIsActive(EntityID entityID);
    bool Entity_GetIsActiveSelf(EntityID entityID);
    void Entity_SetIsActiveSelf(EntityID entityID, bool isActiveSelf);

    bool Entity_IsDestroyed(EntityID entityID);

    UUID_TYPE Entity_GetUUID(EntityID id);
    MonoString* Entity_GetName(EntityID id);
    void Entity_SetName(EntityID id, MonoString* name);

    void Component_FindAllOfType(void* type, bool includeInactive, MonoObject** result);
}
