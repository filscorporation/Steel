#include "EntityInternalCalls.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/NameComponent.h"

EntityID EntityInternalCalls::Entity_CreateNewEntity()
{
    return Application::Instance->GetCurrentScene()->CreateEntity();
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
        Log::LogError("Error getting entity by ID " + std::to_string(id));
        return false;
    }

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(id));
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    bool success = false;
    auto& component = ScriptingCore::AddComponentFromMonoClass(id, monoClass, success);
    if (!success)
    {
        Log::LogError("Error adding component " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return true;
}

bool EntityInternalCalls::Entity_AddScriptComponent(EntityID id, void* type, ScriptPointer scriptPointer)
{
    if (HasComponentS<ScriptComponent>(id))
        return false;

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    auto& scriptComponent = AddComponentS<ScriptComponent>(id);
    scriptComponent.Init(mono_type_get_name(monoType), scriptPointer);

    return true;
}

bool EntityInternalCalls::Entity_HasComponent(EntityID id, void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(id));
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return ScriptingCore::HasComponentFromMonoClass(id, monoClass);
}

ScriptPointer EntityInternalCalls::Entity_GetScriptComponent(EntityID id, void* type)
{
    if (!HasComponentS<ScriptComponent>(id))
        return NULL;

    //MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO: won't work for multiple scripts

    auto& scriptComponent = GetComponentS<ScriptComponent>(id);

    return scriptComponent.GetScriptPointer();
}

bool EntityInternalCalls::Entity_RemoveComponent(EntityID id, void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(id));
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return ScriptingCore::RemoveComponentFromMonoClass(id, monoClass);
}

ScriptPointer EntityInternalCalls::Entity_RemoveScriptComponent(EntityID id, void* type)
{
    if (!HasComponentS<ScriptComponent>(id))
        return NULL;

    //MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO: won't work for multiple scripts

    ScriptPointer scriptPointer = GetComponentS<ScriptComponent>(id).GetScriptPointer();
    RemoveComponentS<ScriptComponent>(id);

    return scriptPointer;
}

MonoString* EntityInternalCalls::Entity_GetName(EntityID id)
{
    if (!HasComponentS<NameComponent>(id))
        return nullptr;

    return mono_string_new(mono_domain_get(), GetComponentS<NameComponent>(id).Name);
}

void EntityInternalCalls::Entity_SetName(EntityID id, MonoString* name)
{
    AddComponentS<NameComponent>(id).Name = mono_string_to_utf8(name);
}
