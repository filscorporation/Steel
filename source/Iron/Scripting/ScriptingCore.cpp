#include "ScriptingCore.h"
#include "../Core/Log.h"
#include "InternalCalls.h"

#include <mono/metadata/debug-helpers.h>

EngineCallsMethods ScriptingCore::EngineCallsMethods;
CachedData* ScriptingCore::cachedData;

void ScriptingCore::Init(MonoImage* image)
{
    LoadEngineCallsMethods(image);
    RegisterInternalCalls();
    CacheAPITypes(image);
}

void ScriptingCore::Terminate()
{
    delete cachedData;
}

void ScriptingCore::LoadEngineCallsMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "Iron", "EngineCalls");

    EngineCallsMethods.callOnCreate = mono_class_get_method_from_name(klass, "ComponentOnCreate", 1);
    EngineCallsMethods.callOnUpdate = mono_class_get_method_from_name(klass, "ComponentOnUpdate", 1);
    EngineCallsMethods.callOnLateUpdate = mono_class_get_method_from_name(klass, "ComponentOnLateUpdate", 1);
    EngineCallsMethods.callOnFixedUpdate = mono_class_get_method_from_name(klass, "ComponentOnFixedUpdate", 1);
}

void ScriptingCore::RegisterInternalCalls()
{
    mono_add_internal_call("Iron.Application::Quit_Internal", (void*)InternalCalls::Application_Quit);

    mono_add_internal_call("Iron.Input::IsKeyPressed_Internal", (void*)InternalCalls::Input_IsKeyPressed);

    mono_add_internal_call("Iron.Entity::CreateNewEntity_Internal", (void*)InternalCalls::Entity_CreateNewEntity);
    mono_add_internal_call("Iron.Entity::AddComponent_Internal", (void*)InternalCalls::Entity_AddComponent);
    mono_add_internal_call("Iron.Entity::AddScriptComponent_Internal", (void*)InternalCalls::Entity_AddScriptComponent);
    mono_add_internal_call("Iron.Entity::GetComponent_Internal", (void*)InternalCalls::Entity_GetComponent);
    mono_add_internal_call("Iron.Entity::GetScriptComponent_Internal", (void*)InternalCalls::Entity_GetScriptComponent);
    mono_add_internal_call("Iron.Entity::RemoveComponent_Internal", (void*)InternalCalls::Entity_RemoveComponent);
    mono_add_internal_call("Iron.Entity::RemoveScriptComponent_Internal", (void*)InternalCalls::Entity_RemoveScriptComponent);

    // TODO: engine calls
}

void ScriptingCore::CacheAPITypes(MonoImage* image)
{
    cachedData = new CachedData();

    CACHE_CLASS(Transformation, API_CLASS(Transformation))
    CACHE_CLASS(BoxCollider, API_CLASS(BoxCollider))
    CACHE_CLASS(RigidBody, API_CLASS(RigidBody))
    CACHE_CLASS(SpriteRenderer, API_CLASS(SpriteRenderer))
    CACHE_CLASS(Camera, API_CLASS(Camera))
    CACHE_CLASS(AudioListener, API_CLASS(AudioListener))
    CACHE_CLASS(AudioSource, API_CLASS(AudioSource))
    CACHE_CLASS(Animator, API_CLASS(Animator))
}

MonoMethod* ScriptingCore::GetMethod(MonoImage* image, const char* methodName)
{
    MonoMethodDesc* description = mono_method_desc_new(methodName, NULL);
    if (!description)
    {
        Log::LogError("Unable to get method description " + std::string(methodName));
        return nullptr;
    }

    MonoMethod* method = mono_method_desc_search_in_image(description, image);
    if (!method)
    {
        Log::LogError("Unable to load method from image " + std::string(methodName));
        return nullptr;
    }

    return method;
}

void ScriptingCore::CallMethod(int64_t scriptID, MonoMethod* method)
{
    MonoObject* exception = nullptr;
    void* params[1];
    params[0] = &scriptID;
    mono_runtime_invoke(method, nullptr, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method " + std::string(mono_method_full_name(method, true)));
        mono_print_unhandled_exception(exception);
    }
}

void ScriptingCore::FindAndCallEntryPoint(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "IronCustom", "GameManager");
    if (klass == nullptr)
    {
        Log::LogError("GameManager class for entry point not found");
        return;
    }

    MonoMethod* entryPointMethod = mono_class_get_method_from_name(klass, "EntryPoint", 0);
    if (entryPointMethod == nullptr)
    {
        Log::LogError("No entry point found");
        return;
    }

    MonoObject* exception = nullptr;
    mono_runtime_invoke(entryPointMethod, nullptr, nullptr, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling entry point");
        mono_print_unhandled_exception(exception);
    }
}
