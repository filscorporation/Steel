#include <mono/metadata/debug-helpers.h>

#include "ScriptingCore.h"
#include "ScriptingCallsRegister.h"
#include "../Animation/Animator.h"
#include "../Audio/AudioListener.h"
#include "../Audio/AudioSource.h"
#include "../Core/Log.h"
#include "../Physics/BoxCollider.h"
#include "../Physics/CircleCollider.h"
#include "../Physics/RigidBody.h"
#include "../Rendering/Camera.h"
#include "../Rendering/SpriteRenderer.h"
#include "../UI/RectTransformation.h"
#include "../UI/UIRenderer.h"
#include "../UI/UIImage.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/NameComponent.h"

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
    CACHE_CLASS(NameComponent, API_CLASS(NameComponent))
    CACHE_CLASS(BoxCollider, API_CLASS(BoxCollider))
    CACHE_CLASS(CircleCollider, API_CLASS(CircleCollider))
    CACHE_CLASS(RigidBody, API_CLASS(RigidBody))
    CACHE_CLASS(SpriteRenderer, API_CLASS(SpriteRenderer))
    CACHE_CLASS(Camera, API_CLASS(Camera))
    CACHE_CLASS(AudioListener, API_CLASS(AudioListener))
    CACHE_CLASS(AudioSource, API_CLASS(AudioSource))
    CACHE_CLASS(Animator, API_CLASS(Animator))
    CACHE_CLASS(RectTransformation, API_CLASS(RectTransformation))
    CACHE_CLASS(UIRenderer, API_CLASS(UIRenderer))
    CACHE_CLASS(UIImage, API_CLASS(UIImage))
}

Component& ScriptingCore::AddComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success)
{
    success = true;
    if (monoClass == CACHED_CLASS(NameComponent))
        return AddComponentS<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return AddComponentS<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return AddComponentS<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return AddComponentS<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return AddComponentS<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return AddComponentS<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return AddComponentS<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return AddComponentS<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return AddComponentS<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return AddComponentS<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return AddComponentS<Animator>(entity);
    if (monoClass == CACHED_CLASS(UIRenderer))
        return AddComponentS<UIRenderer>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return AddComponentS<UIImage>(entity);

    Log::LogError("Could not find cached class");

    success = false;
    auto nullComponent = Component(NULL_ENTITY);
    return nullComponent; // Warning can be ignored
}

bool ScriptingCore::HasComponentFromMonoClass(EntityID entity, MonoClass *monoClass)
{
    if (monoClass == CACHED_CLASS(NameComponent))
        return HasComponentS<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return HasComponentS<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return HasComponentS<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return HasComponentS<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return HasComponentS<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return HasComponentS<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return HasComponentS<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return HasComponentS<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return HasComponentS<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return HasComponentS<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return HasComponentS<Animator>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return HasComponentS<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(UIRenderer))
        return HasComponentS<UIRenderer>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return HasComponentS<UIImage>(entity);

    Log::LogError("Could not find cached class");

    return false;
}

bool ScriptingCore::RemoveComponentFromMonoClass(EntityID entity, MonoClass *monoClass)
{
    if (monoClass == CACHED_CLASS(NameComponent))
        return RemoveComponentS<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return RemoveComponentS<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return RemoveComponentS<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return RemoveComponentS<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return RemoveComponentS<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return RemoveComponentS<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return RemoveComponentS<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return RemoveComponentS<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return RemoveComponentS<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return RemoveComponentS<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return RemoveComponentS<Animator>(entity);
    if (monoClass == CACHED_CLASS(UIRenderer))
        return RemoveComponentS<UIRenderer>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return RemoveComponentS<UIImage>(entity);

    Log::LogError("Could not find cached class");

    return false;
}

std::vector<EntityID> ScriptingCore::ComponentOwnersFromMonoClass(MonoClass* monoClass)
{
    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (monoClass == CACHED_CLASS(NameComponent))
        RETURN_COMPONENT_OWNERS(NameComponent)
    if (monoClass == CACHED_CLASS(Transformation))
        RETURN_COMPONENT_OWNERS(Transformation)
    if (monoClass == CACHED_CLASS(RectTransformation))
        RETURN_COMPONENT_OWNERS(RectTransformation)
    if (monoClass == CACHED_CLASS(BoxCollider))
        RETURN_COMPONENT_OWNERS(BoxCollider)
    if (monoClass == CACHED_CLASS(CircleCollider))
        RETURN_COMPONENT_OWNERS(CircleCollider)
    if (monoClass == CACHED_CLASS(RigidBody))
        RETURN_COMPONENT_OWNERS(RigidBody)
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        RETURN_COMPONENT_OWNERS(SpriteRenderer)
    if (monoClass == CACHED_CLASS(Camera))
        RETURN_COMPONENT_OWNERS(Camera)
    if (monoClass == CACHED_CLASS(AudioListener))
        RETURN_COMPONENT_OWNERS(AudioListener)
    if (monoClass == CACHED_CLASS(AudioSource))
        RETURN_COMPONENT_OWNERS(AudioSource)
    if (monoClass == CACHED_CLASS(Animator))
        RETURN_COMPONENT_OWNERS(Animator)
    if (monoClass == CACHED_CLASS(UIRenderer))
        RETURN_COMPONENT_OWNERS(UIRenderer)
    if (monoClass == CACHED_CLASS(UIImage))
        RETURN_COMPONENT_OWNERS(UIImage)

    Log::LogError("Could not find cached class");

    return std::vector<EntityID>();
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

void ScriptingCore::CallMethod(ScriptPointer scriptPointer, MonoMethod* method)
{
    if (scriptPointer == 0)
    {
        Log::LogError("Trying to call method in null pointer object");
        return;
    }

    MonoObject* exception = nullptr;
    void* params[1];
    params[0] = &scriptPointer;
    mono_runtime_invoke(method, nullptr, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method " + std::string(mono_method_full_name(method, true)));
        mono_print_unhandled_exception(exception);
    }
}

void ScriptingCore::CallMethod(ScriptPointer scriptPointer, MonoMethod *method, EntityID param)
{
    if (scriptPointer == 0)
    {
        Log::LogError("Trying to call method in null pointer object");
        return;
    }

    MonoObject* exception = nullptr;
    void* params[2];
    params[0] = &scriptPointer;
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

MonoArray* ScriptingCore::ToMonoUInt32Array(const std::vector<uint32_t> &inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_uint32_class(), inArray.size());

    for (int i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, uint32_t, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoIntPtrArray(const std::vector<intptr_t>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_intptr_class(), inArray.size());

    for (int i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, intptr_t, i, inArray[i]);
    }

    return outArray;
}
