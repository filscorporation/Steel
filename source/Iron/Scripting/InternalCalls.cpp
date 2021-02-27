#include "InternalCalls.h"
#include "../Core/Application.h"
#include "ScriptComponent.h"
#include "../Core/Log.h"

#include <mono/jit/jit.h>

void InternalCalls::Application_Quit()
{
    Application::Instance->Quit();
}

bool InternalCalls::Input_IsKeyPressed(int code)
{
    return Input::IsKeyPressed((KeyCodes::KeyCode)code);
}

uint64_t InternalCalls::Entity_CreateNewEntity()
{
    return Application::Instance->GetCurrentScene()->CreateEntity()->ID;
}

uint64_t InternalCalls::Entity_AddComponent(uint64_t id, void* type)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
    {
        Log::LogError("Error getting entity by ID " + std::to_string(id));
        return 0;
    }

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(id));
        return 0;
    }
    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return 0;
    }

    // TODO
    return 0;
}

uint64_t InternalCalls::Entity_AddScriptComponent(uint64_t id, void* type, int64_t scriptPointer)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
        return 0;

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    auto scriptComponent = entity->AddComponent<ScriptComponent>();

    if (scriptComponent == nullptr)
        return 0;

    scriptComponent->Init(mono_type_get_name(monoType), scriptPointer);

    return scriptComponent->ID;
}

uint64_t InternalCalls::Entity_GetComponent(uint64_t id, void* type)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
        return 0;

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO
    return 0;
}

uint64_t InternalCalls::Entity_GetScriptComponent(uint64_t id, void* type)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
        return 0;

    //MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO: won't work for multiple scripts; create map

    auto scriptComponent = entity->GetComponent<ScriptComponent>();
    if (scriptComponent == nullptr)
        return 0;

    return scriptComponent->ID;
}

uint64_t InternalCalls::Entity_RemoveComponent(uint64_t id, void* type)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
        return 0;

    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO
    return 0;
}

uint64_t InternalCalls::Entity_RemoveScriptComponent(uint64_t id, void* type)
{
    Entity* entity = Application::Instance->GetCurrentScene()->GetEntity(id);
    if (entity == nullptr)
        return 0;

    //MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    // TODO: won't work for multiple scripts; create map

    auto scriptComponent = entity->GetComponent<ScriptComponent>();
    if (scriptComponent == nullptr)
        return 0;

    uint64_t componentID = scriptComponent->ID;
    entity->RemoveComponent<ScriptComponent>();

    return componentID;
}
