#pragma once

#include <cstdint>
#include "ScriptingCore.h"
#include "ScriptComponent.h"

namespace EntityInternalCalls
{
    EntityID Entity_CreateNewEntity();
    bool Entity_AddComponent(EntityID id, void* type);
    bool Entity_AddScriptComponent(EntityID id, void* type, ScriptPointer scriptPointer);
    bool Entity_HasComponent(EntityID id, void* type);
    ScriptPointer Entity_GetScriptComponent(EntityID id, void* type);
    bool Entity_RemoveComponent(EntityID id, void* type);
    ScriptPointer Entity_RemoveScriptComponent(EntityID id, void* type);

    MonoString* Entity_GetName(EntityID id);
    void Entity_SetName(EntityID id, MonoString* name);
}
