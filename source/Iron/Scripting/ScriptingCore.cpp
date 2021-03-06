#include "ScriptingCore.h"
#include "../Animation/Animator.h"
#include "../Audio/AudioListener.h"
#include "../Audio/AudioSource.h"
#include "../Core/Log.h"
#include "../Physics/BoxCollider.h"
#include "../Physics/RigidBody.h"
#include "../Rendering/Camera.h"
#include "../Rendering/SpriteRenderer.h"
#include "ScriptingCallsRegister.h"

#include <mono/metadata/debug-helpers.h>

EngineCallsMethods ScriptingCore::EngineCalls;
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
    MonoClass* klass = mono_class_from_name(image, "Iron", "ComponentEngineCalls");

    EngineCalls.callOnCreate = mono_class_get_method_from_name(klass, "ComponentOnCreate", 1);
    EngineCalls.callOnUpdate = mono_class_get_method_from_name(klass, "ComponentOnUpdate", 1);
    EngineCalls.callOnLateUpdate = mono_class_get_method_from_name(klass, "ComponentOnLateUpdate", 1);
    EngineCalls.callOnFixedUpdate = mono_class_get_method_from_name(klass, "ComponentOnFixedUpdate", 1);

    EngineCalls.callOnCollisionEnter = mono_class_get_method_from_name(klass, "ComponentOnCollisionEnter", 2);
    EngineCalls.callOnCollisionStay = mono_class_get_method_from_name(klass, "ComponentOnCollisionStay", 2);
    EngineCalls.callOnCollisionExit = mono_class_get_method_from_name(klass, "ComponentOnCollisionExit", 2);

    EngineCalls.callOnMouseOver = mono_class_get_method_from_name(klass, "ComponentOnMouseOver", 1);
    EngineCalls.callOnMouseEnter = mono_class_get_method_from_name(klass, "ComponentOnMouseEnter", 1);
    EngineCalls.callOnMouseExit = mono_class_get_method_from_name(klass, "ComponentOnMouseExit", 1);
    EngineCalls.callOnMousePressed = mono_class_get_method_from_name(klass, "ComponentOnMousePressed", 1);
    EngineCalls.callOnMouseJustPressed = mono_class_get_method_from_name(klass, "ComponentOnMouseJustPressed", 1);
    EngineCalls.callOnMouseJustReleased = mono_class_get_method_from_name(klass, "ComponentOnMouseJustReleased", 1);
}

void ScriptingCore::RegisterInternalCalls()
{
    ScriptingCallsRegister::RegisterInternalCalls();
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

Component* ScriptingCore::AddComponentFromMonoClass(Entity* entity, MonoClass* monoClass)
{
    if (monoClass == CACHED_CLASS(Transformation))
        return entity->AddComponent<Transformation>();
    if (monoClass == CACHED_CLASS(BoxCollider))
        return entity->AddComponent<BoxCollider>();
    if (monoClass == CACHED_CLASS(RigidBody))
        return entity->AddComponent<RigidBody>();
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return entity->AddComponent<SpriteRenderer>();
    if (monoClass == CACHED_CLASS(Camera))
        return entity->AddComponent<Camera>();
    if (monoClass == CACHED_CLASS(AudioListener))
        return entity->AddComponent<AudioListener>();
    if (monoClass == CACHED_CLASS(AudioSource))
        return entity->AddComponent<AudioSource>();
    if (monoClass == CACHED_CLASS(Animator))
        return entity->AddComponent<Animator>();

    Log::LogError("Could not find cached class");

    return nullptr;
}

Component *ScriptingCore::GetComponentFromMonoClass(Entity *entity, MonoClass *monoClass)
{
    if (monoClass == CACHED_CLASS(Transformation))
        return entity->GetComponent<Transformation>();
    if (monoClass == CACHED_CLASS(BoxCollider))
        return entity->GetComponent<BoxCollider>();
    if (monoClass == CACHED_CLASS(RigidBody))
        return entity->GetComponent<RigidBody>();
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return entity->GetComponent<SpriteRenderer>();
    if (monoClass == CACHED_CLASS(Camera))
        return entity->GetComponent<Camera>();
    if (monoClass == CACHED_CLASS(AudioListener))
        return entity->GetComponent<AudioListener>();
    if (monoClass == CACHED_CLASS(AudioSource))
        return entity->GetComponent<AudioSource>();
    if (monoClass == CACHED_CLASS(Animator))
        return entity->GetComponent<Animator>();

    Log::LogError("Could not find cached class");

    return nullptr;
}

int64_t ScriptingCore::RemoveComponentFromMonoClass(Entity *entity, MonoClass *monoClass)
{
    Component* component;
    if (monoClass == CACHED_CLASS(Transformation))
    {
        component = entity->GetComponent<Transformation>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<Transformation>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(BoxCollider))
    {
        component = entity->GetComponent<BoxCollider>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<BoxCollider>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(RigidBody))
    {
        component = entity->GetComponent<RigidBody>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<RigidBody>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(SpriteRenderer))
    {
        component = entity->GetComponent<SpriteRenderer>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<SpriteRenderer>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(Camera))
    {
        component = entity->GetComponent<Camera>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<Camera>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(AudioListener))
    {
        component = entity->GetComponent<AudioListener>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<AudioListener>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(AudioSource))
    {
        component = entity->GetComponent<AudioSource>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<AudioSource>();
        return component->ID;
    }
    if (monoClass == CACHED_CLASS(Animator))
    {
        component = entity->GetComponent<Animator>();
        if (component == nullptr)
            return 0;
        entity->RemoveComponent<Animator>();
        return component->ID;
    }

    Log::LogError("Could not find cached class");

    return 0;
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

void ScriptingCore::CallMethod(int64_t scriptID, MonoMethod *method, int64_t param)
{
    MonoObject* exception = nullptr;
    void* params[2];
    params[0] = &scriptID;
    params[1] = &param;
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
