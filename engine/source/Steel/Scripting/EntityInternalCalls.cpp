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

MonoObject* EntityInternalCalls::Entity_AddComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return nullptr;
    }

    return ScriptingCore::AddComponentFromType(id, type);
}

MonoObject* EntityInternalCalls::Entity_GetComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return nullptr;
    }

    return ScriptingCore::GetComponentFromType(id, type);
}

bool EntityInternalCalls::Entity_HasComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    return ScriptingCore::HasComponentFromType(id, type);
}

bool EntityInternalCalls::Entity_RemoveComponent(EntityID id, void* type)
{
    if (!Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntityExists(id))
    {
        Log::LogError("Error getting entity by ID {0}", id);
        return false;
    }

    return ScriptingCore::RemoveComponentFromType(id, type);
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

UUID_TYPE EntityInternalCalls::Entity_GetUUID(EntityID id)
{
    if (!HasComponentS<IDComponent>(id))
        return NULL_UUID;

    return GetComponentS<IDComponent>(id).GetUUID();
}

MonoString* EntityInternalCalls::Entity_GetName(EntityID id)
{
    if (!HasComponentS<NameComponent>(id))
        return nullptr;

    return ScriptingCore::FromString(GetComponentS<NameComponent>(id).GetName().c_str());
}

void EntityInternalCalls::Entity_SetName(EntityID id, MonoString* name)
{
    AddComponentS<NameComponent>(id).SetName(ScriptingCore::ToString(name));
}

void EntityInternalCalls::Component_FindAllOfType(void* type, bool includeInactive, MonoObject** result)
{
    ScriptingCore::GetComponentsListFromType(type, includeInactive, result);
}
