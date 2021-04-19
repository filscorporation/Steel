#include <mono/metadata/debug-helpers.h>

#include "ScriptingCore.h"
#include "ScriptingCallsRegister.h"
#include "ScriptComponentSystem.h"
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
#include "../UI/UIElements/UIImage.h"
#include "../UI/UIElements/UIButton.h"
#include "../UI/UIElements/UIText.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/NameComponent.h"

EngineCallsMethods ScriptingCore::EngineCalls;
EventManagerMethods ScriptingCore::EventManagerCalls;
CoroutinesManagerMethods ScriptingCore::CoroutinesManagerCalls;
CachedData* ScriptingCore::cachedAPITypes;
std::vector<MonoClass*> ScriptingCore::cachedDataTypes;
std::unordered_map<MonoClass*, ScriptTypeInfo*> ScriptingCore::scriptsInfo;
ScriptComponentSystem* ScriptingCore::scriptComponentSystem;
Component nullComponent = Component(NULL_ENTITY);

void ScriptingCore::Init(MonoImage* image)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    scriptComponentSystem = new ScriptComponentSystem();
    registry->RegisterSystem<ScriptComponent>(scriptComponentSystem);

    LoadEngineCallsMethods(image);
    LoadEventManagerMethods(image);
    LoadCoroutinesManagerMethods(image);
    RegisterInternalCalls();
    CacheAPITypes(image);
    CacheDataTypes(image);
}

void ScriptingCore::Terminate()
{
    for (auto info : scriptsInfo)
        delete info.second;
    delete cachedAPITypes;
    delete scriptComponentSystem;
}

void ScriptingCore::LoadEngineCallsMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "Iron", "ComponentEngineCalls");

    EngineCalls.freeScriptHandle = mono_class_get_method_from_name(klass, "FreeScriptHandle", 1);

    EngineCalls.callOnCreate = mono_class_get_method_from_name(klass, "ComponentOnCreate", 1);
    EngineCalls.callOnDestroy = mono_class_get_method_from_name(klass, "ComponentOnDestroy", 1);
    EngineCalls.callOnUpdate = mono_class_get_method_from_name(klass, "ComponentOnUpdate", 1);
    EngineCalls.callOnLateUpdate = mono_class_get_method_from_name(klass, "ComponentOnLateUpdate", 1);
    EngineCalls.callOnFixedUpdate = mono_class_get_method_from_name(klass, "ComponentOnFixedUpdate", 1);
    EngineCalls.callOnEnabled = mono_class_get_method_from_name(klass, "ComponentOnEnabled", 1);
    EngineCalls.callOnDisabled = mono_class_get_method_from_name(klass, "ComponentOnDisabled", 1);

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

void ScriptingCore::LoadEventManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "Iron", "EventManager");

    EventManagerCalls.callInvokeCallbacks = mono_class_get_method_from_name(klass, "InvokeCallbacks", 1);
    EventManagerCalls.callDeregisterCallbacks = mono_class_get_method_from_name(klass, "DeregisterCallbacks", 1);
}

void ScriptingCore::LoadCoroutinesManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "Iron", "CoroutinesManager");

    CoroutinesManagerCalls.callUpdate = mono_class_get_method_from_name(klass, "Update", 0);
}

void ScriptingCore::RegisterInternalCalls()
{
    ScriptingCallsRegister::RegisterInternalCalls();
}

void ScriptingCore::CacheAPITypes(MonoImage* image)
{
    cachedAPITypes = new CachedData();

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
    CACHE_CLASS(UIImage, API_CLASS(UIImage))
    CACHE_CLASS(UIButton, API_CLASS(UIButton))
    CACHE_CLASS(UIText, API_CLASS(UIText))
}

void ScriptingCore::CacheDataTypes(MonoImage* image)
{
    cachedDataTypes.push_back(API_CLASS(RayCastHit));
}

void ScriptingCore::FreeScriptHandle(ScriptPointer scriptPointer)
{
    if (scriptPointer == 0)
    {
        Log::LogError("Trying to free null pointer script");
        return;
    }

    MonoObject* exception = nullptr;
    void* params[1];
    params[0] = &scriptPointer;
    mono_runtime_invoke(ScriptingCore::EngineCalls.freeScriptHandle, nullptr, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error freeing script handle");
        mono_print_unhandled_exception(exception);
    }
}

Component& ScriptingCore::AddComponentFromType(EntityID entity, void* type, bool& success)
{
    success = false;
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(entity));
        return nullComponent;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return nullComponent;
    }

    auto& component = ScriptingCore::AddComponentFromMonoClass(entity, monoClass, success);
    if (!success)
    {
        Log::LogError("Error adding component " + std::string(mono_type_get_name(monoType)));
        return nullComponent;
    }

    success = true;
    return nullComponent;
}

Component& ScriptingCore::AddComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success)
{
    success = true;
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (monoClass == CACHED_CLASS(NameComponent))
        return entitiesRegistry->AddComponent<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return entitiesRegistry->AddComponent<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return entitiesRegistry->AddComponent<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return entitiesRegistry->AddComponent<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return entitiesRegistry->AddComponent<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return entitiesRegistry->AddComponent<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return entitiesRegistry->AddComponent<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return entitiesRegistry->AddComponent<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return entitiesRegistry->AddComponent<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return entitiesRegistry->AddComponent<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return entitiesRegistry->AddComponent<Animator>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return entitiesRegistry->AddComponent<UIImage>(entity);
    if (monoClass == CACHED_CLASS(UIButton))
        return entitiesRegistry->AddComponent<UIButton>(entity);
    if (monoClass == CACHED_CLASS(UIText))
        return entitiesRegistry->AddComponent<UIText>(entity);

    Log::LogError("Could not find cached class");

    success = false;
    return nullComponent;
}

bool ScriptingCore::HasComponentFromType(EntityID entity, void* type, bool& success)
{
    success = false;
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(entity));
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    success = true;
    return ScriptingCore::HasComponentFromMonoClass(entity, monoClass);
}

bool ScriptingCore::HasComponentFromMonoClass(EntityID entity, MonoClass *monoClass)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    
    if (monoClass == CACHED_CLASS(NameComponent))
        return entitiesRegistry->HasComponent<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return entitiesRegistry->HasComponent<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return entitiesRegistry->HasComponent<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return entitiesRegistry->HasComponent<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return entitiesRegistry->HasComponent<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return entitiesRegistry->HasComponent<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return entitiesRegistry->HasComponent<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return entitiesRegistry->HasComponent<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return entitiesRegistry->HasComponent<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return entitiesRegistry->HasComponent<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return entitiesRegistry->HasComponent<Animator>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return entitiesRegistry->HasComponent<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return entitiesRegistry->HasComponent<UIImage>(entity);
    if (monoClass == CACHED_CLASS(UIButton))
        return entitiesRegistry->HasComponent<UIButton>(entity);
    if (monoClass == CACHED_CLASS(UIText))
        return entitiesRegistry->HasComponent<UIText>(entity);

    Log::LogError("Could not find cached class");

    return false;
}

bool ScriptingCore::RemoveComponentFromType(EntityID entity, void* type, bool& success)
{
    success = false;
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for entity ID " + std::to_string(entity));
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    success = true;
    return ScriptingCore::RemoveComponentFromMonoClass(entity, monoClass);
}

bool ScriptingCore::RemoveComponentFromMonoClass(EntityID entity, MonoClass *monoClass)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    
    if (monoClass == CACHED_CLASS(NameComponent))
        return entitiesRegistry->RemoveComponent<NameComponent>(entity);
    if (monoClass == CACHED_CLASS(Transformation))
        return entitiesRegistry->RemoveComponent<Transformation>(entity);
    if (monoClass == CACHED_CLASS(RectTransformation))
        return entitiesRegistry->RemoveComponent<RectTransformation>(entity);
    if (monoClass == CACHED_CLASS(BoxCollider))
        return entitiesRegistry->RemoveComponent<BoxCollider>(entity);
    if (monoClass == CACHED_CLASS(CircleCollider))
        return entitiesRegistry->RemoveComponent<CircleCollider>(entity);
    if (monoClass == CACHED_CLASS(RigidBody))
        return entitiesRegistry->RemoveComponent<RigidBody>(entity);
    if (monoClass == CACHED_CLASS(SpriteRenderer))
        return entitiesRegistry->RemoveComponent<SpriteRenderer>(entity);
    if (monoClass == CACHED_CLASS(Camera))
        return entitiesRegistry->RemoveComponent<Camera>(entity);
    if (monoClass == CACHED_CLASS(AudioListener))
        return entitiesRegistry->RemoveComponent<AudioListener>(entity);
    if (monoClass == CACHED_CLASS(AudioSource))
        return entitiesRegistry->RemoveComponent<AudioSource>(entity);
    if (monoClass == CACHED_CLASS(Animator))
        return entitiesRegistry->RemoveComponent<Animator>(entity);
    if (monoClass == CACHED_CLASS(UIImage))
        return entitiesRegistry->RemoveComponent<UIImage>(entity);
    if (monoClass == CACHED_CLASS(UIButton))
        return entitiesRegistry->RemoveComponent<UIButton>(entity);
    if (monoClass == CACHED_CLASS(UIText))
        return entitiesRegistry->RemoveComponent<UIText>(entity);

    Log::LogError("Could not find cached class");

    return false;
}

bool ScriptingCore::ComponentOwnersFromType(void* type, std::vector<EntityID>& result)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type for find");
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    ScriptingCore::ComponentOwnersFromMonoClass(monoClass, result);
    return true;
}

bool ScriptingCore::ComponentOwnersFromMonoClass(MonoClass* monoClass, std::vector<EntityID>& result)
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
    if (monoClass == CACHED_CLASS(UIImage))
        RETURN_COMPONENT_OWNERS(UIImage)
    if (monoClass == CACHED_CLASS(UIButton))
        RETURN_COMPONENT_OWNERS(UIButton)
    if (monoClass == CACHED_CLASS(UIText))
        RETURN_COMPONENT_OWNERS(UIText)

    Log::LogError("Could not find cached class");

    return false;
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

void ScriptingCore::CallEventMethod(EntityID ownerEntityID, MonoMethod* method)
{
    if (ownerEntityID == NULL_ENTITY)
    {
        Log::LogError("Trying to call method in null entity");
        return;
    }

    MonoObject* exception = nullptr;
    void* params[1];
    params[0] = &ownerEntityID;
    mono_runtime_invoke(method, nullptr, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method " + std::string(mono_method_full_name(method, true)));
        mono_print_unhandled_exception(exception);
    }
}

void ScriptingCore::CallMethod(MonoMethod* method)
{
    MonoObject* exception = nullptr;
    mono_runtime_invoke(method, nullptr, nullptr, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method " + std::string(mono_method_full_name(method, true)));
        mono_print_unhandled_exception(exception);
    }
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

void ScriptingCore::CallMethod(ScriptPointer scriptPointer, MonoMethod* method, EntityID param)
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
    Log::LogInfo("FindAndCallEntryPoint");

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
    mono_runtime_invoke(entryPointMethod, nullptr, nullptr,& exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling entry point");
        mono_print_unhandled_exception(exception);
    }
}

MonoArray* ScriptingCore::ToMonoUInt32Array(const std::vector<uint32_t>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_uint32_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, uint32_t, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoIntPtrArray(const std::vector<intptr_t>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_intptr_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, intptr_t, i, inArray[i]);
    }

    return outArray;
}

void ScriptingCore::FromMonoUInt32Array(MonoArray* inArray, std::vector<uint32_t>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.push_back(mono_array_get(inArray, uint32_t, i));
    }
}

bool ScriptingCore::AddScriptComponentFromType(EntityID entity, ScriptPointer scriptPointer, void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return AddScriptComponentFromMonoClass(entity, scriptPointer, monoClass);
}

bool ScriptingCore::AddScriptComponentFromMonoClass(EntityID entity, ScriptPointer scriptPointer, MonoClass* monoClass)
{
    ScriptTypeInfo* typeInfo;
    if (scriptsInfo.find(monoClass) == scriptsInfo.end())
    {
        typeInfo = new ScriptTypeInfo();
        typeInfo->Mask = ScriptGetEventMask(monoClass);
        scriptsInfo[monoClass] = typeInfo;
    }
    else
    {
        typeInfo = scriptsInfo[monoClass];
    }

    auto& scriptComponent = AddComponentS<ScriptComponent>(entity);
    if (scriptComponent.HasScriptType(typeInfo))
        // We can support multiple script instances of same type later if neede, for now return false
        return false;

    scriptComponent.AddScript(scriptPointer, typeInfo);

    return true;
}

bool ScriptingCore::HasScriptComponentFromType(EntityID entity, void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return HasScriptComponentFromMonoClass(entity, monoClass);
}

bool ScriptingCore::HasScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass)
{
    ScriptTypeInfo* typeInfo;
    if (scriptsInfo.find(monoClass) == scriptsInfo.end())
    {
        return false;
    }
    else
    {
        typeInfo = scriptsInfo[monoClass];
    }

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entity))
        return false;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entity);

    return scriptComponent.HasScriptType(typeInfo);
}

ScriptPointer ScriptingCore::GetScriptComponentFromType(EntityID entity, void* type, bool& success)
{
    success = false;
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return 0;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return 0;
    }

    return GetScriptComponentFromMonoClass(entity, monoClass, success);
}

ScriptPointer ScriptingCore::GetScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success)
{
    success = false;
    ScriptTypeInfo* typeInfo;
    if (scriptsInfo.find(monoClass) == scriptsInfo.end())
    {
        return 0;
    }
    else
    {
        typeInfo = scriptsInfo[monoClass];
    }

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entity))
        return 0;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entity);

    if (!scriptComponent.HasScriptType(typeInfo))
        return 0;

    success = true;
    return scriptComponent.GetScriptPointer(typeInfo);
}

bool ScriptingCore::RemoveScriptComponentFromType(EntityID entity, void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    return RemoveScriptComponentFromMonoClass(entity, monoClass);
}

bool ScriptingCore::RemoveScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass)
{
    ScriptTypeInfo* typeInfo;
    if (scriptsInfo.find(monoClass) == scriptsInfo.end())
    {
        return false;
    }
    else
    {
        typeInfo = scriptsInfo[monoClass];
    }

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entity))
        return false;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entity);
    if (!scriptComponent.HasScriptType(typeInfo))
        return false;

    scriptComponent.RemoveScript(typeInfo);
    if (scriptComponent.Scripts.empty())
        entitiesRegistry->RemoveComponent<ScriptComponent>(entity);

    return true;
}

bool ScriptingCore::ScriptComponentPointersFromType(void* type, std::vector<ScriptPointer>& result)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return false;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: " + std::string(mono_type_get_name(monoType)));
        return false;
    }

    ScriptComponentPointersFromMonoClass(monoClass, result);

    return true;
}

void ScriptingCore::ScriptComponentPointersFromMonoClass(MonoClass* monoClass, std::vector<ScriptPointer>& result)
{
    ScriptTypeInfo* typeInfo;
    if (scriptsInfo.find(monoClass) == scriptsInfo.end())
        return;
    typeInfo = scriptsInfo[monoClass];

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto iterator = entitiesRegistry->GetComponentIterator<ScriptComponent>();
    for (auto& script : iterator)
    {
        if (script.HasScriptType(typeInfo))
            result.push_back(script.GetScriptPointer(typeInfo));
    }
}

ScriptEventTypes::ScriptEventType ScriptingCore::ScriptGetEventMask(MonoClass* monoClass)
{
    return (ScriptEventTypes::ScriptEventType)0xFFFFFFFF; // TODO: calculate mask
}
