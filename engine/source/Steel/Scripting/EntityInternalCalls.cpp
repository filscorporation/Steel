#include <mono/jit/jit.h>

#include "EntityInternalCalls.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/NameComponent.h"

EntityID EntityInternalCalls::Entity_CreateNewEntity()
{
    return Application::Instance->GetCurrentScene()->CreateEntity();
}

EntityID EntityInternalCalls::Entity_CreateNewEntity2(MonoString* name)
{
    return Application::Instance->GetCurrentScene()->CreateEntity(ScriptingCore::ToString(name), NULL_ENTITY);
}

EntityID EntityInternalCalls::Entity_CreateNewEntity3(MonoString* name, EntityID parentEntityID)
{
    return Application::Instance->GetCurrentScene()->CreateEntity(ScriptingCore::ToString(name), parentEntityID);
}

bool EntityInternalCalls::Entity_DestroyEntity(EntityID id)
{
    Application::Instance->GetCurrentScene()->DestroyEntity(id);
    return true;
}

bool EntityInternalCalls::Entity_AddComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    bool success = false;
    ScriptingCore::AddComponentFromType(id, type, success);
    if (!success)
        return false;

    return true;
}

bool EntityInternalCalls::Entity_AddScriptComponent(EntityID id, void* type, ScriptPointer scriptPointer)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    return ScriptingCore::AddScriptComponentFromType(id, type, scriptPointer);
}

bool EntityInternalCalls::Entity_HasComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    bool success = false;
    bool result = ScriptingCore::HasComponentFromType(id, type, success);
    if (!success)
        Log::LogError("Error getting component for entity {0}", id);

    return result;
}

bool EntityInternalCalls::Entity_HasScriptComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    return ScriptingCore::HasScriptComponentFromType(id, type);
}

ScriptPointer EntityInternalCalls::Entity_GetScriptComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    bool success = false;
    return ScriptingCore::GetScriptComponentFromType(id, type, success);
}

bool EntityInternalCalls::Entity_RemoveComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    bool success = false;
    bool result = ScriptingCore::RemoveComponentFromType(id, type, success);
    if (!success)
        return false;

    return result;
}

bool EntityInternalCalls::Entity_RemoveScriptComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    return ScriptingCore::RemoveScriptComponentFromType(id, type);
}

bool EntityInternalCalls::Entity_GetIsActive(EntityID entityID)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    return registry->EntityGetState(entityID) & EntityStates::IsActive;
}

bool EntityInternalCalls::Entity_GetIsActiveSelf(EntityID entityID)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    return registry->EntityGetState(entityID) & EntityStates::IsActiveSelf;
}

void EntityInternalCalls::Entity_SetIsActiveSelf(EntityID entityID, bool isActiveSelf)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    registry->EntitySetActive(entityID, isActiveSelf, true);
}

bool EntityInternalCalls::Entity_IsDestroyed(EntityID entityID)
{
    return !Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(entityID);
}

MonoString* EntityInternalCalls::Entity_GetName(EntityID id)
{
    if (!HasComponentS<NameComponent>(id))
        return nullptr;

    return mono_string_new(mono_domain_get(), GetComponentS<NameComponent>(id).Name.c_str());
}

void EntityInternalCalls::Entity_SetName(EntityID id, MonoString* name)
{
    AddComponentS<NameComponent>(id).Name = ScriptingCore::ToString(name);
}

MonoArray* EntityInternalCalls::Component_FindAllOfType(void* type)
{
    std::vector<EntityID> result;
    if (!ScriptingCore::ComponentOwnersFromType(type, result))
        return nullptr;

    return ScriptingCore::ToMonoUInt32Array(result);
}

MonoArray* EntityInternalCalls::Component_FindAllScriptsOfType(void* type)
{
    std::vector<ScriptPointer> result;
    if (!ScriptingCore::ScriptComponentPointersFromType(type, result))
        return nullptr;

    return ScriptingCore::ToMonoIntPtrArray(result);
}
