#pragma once

#include <cstdint>

#include "ScriptingCore.h"
#include "ScriptComponent.h"

namespace EntityInternalCalls
{
    EntityID Entity_CreateNewEntity();
    EntityID Entity_CreateNewEntity2(MonoString* name);
    EntityID Entity_CreateNewEntity3(MonoString* name, EntityID parentEntityID);
    bool Entity_DestroyEntity(EntityID id);
    bool Entity_AddComponent(EntityID id, void* type);
    bool Entity_AddScriptComponent(EntityID id, void* type, ScriptPointer scriptPointer);
    bool Entity_HasComponent(EntityID id, void* type);
    bool Entity_HasScriptComponent(EntityID id, void* type);
    ScriptPointer Entity_GetScriptComponent(EntityID id, void* type);
    bool Entity_RemoveComponent(EntityID id, void* type);
    bool Entity_RemoveScriptComponent(EntityID id, void* type);

    bool Entity_GetIsActive(EntityID entityID);
    bool Entity_GetIsActiveSelf(EntityID entityID);
    void Entity_SetIsActiveSelf(EntityID entityID, bool isActiveSelf);

    bool Entity_IsDestroyed(EntityID entityID);

    MonoString* Entity_GetName(EntityID id);
    void Entity_SetName(EntityID id, MonoString* name);

    MonoArray* Component_FindAllOfType(void* type);
    MonoArray* Component_FindAllScriptsOfType(void* type);
}
