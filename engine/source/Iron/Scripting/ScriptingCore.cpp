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
#include "../UI/UIElements/UIInputField.h"
#include "../UI/UIElements/UIClipping.h"
#include "../UI/UIElements/UILayoutGroup.h"
#include "../UI/UIElements/UITabs.h"
#include "../UI/UIElements/UIText.h"
#include "../UI/UIElements/UICheckBox.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/NameComponent.h"

EngineCallsMethods ScriptingCore::EngineCalls;
EventManagerMethods ScriptingCore::EventManagerCalls;
CoroutinesManagerMethods ScriptingCore::CoroutinesManagerCalls;
CachedData* ScriptingCore::cachedAPITypes;
std::vector<MonoClass*> ScriptingCore::cachedDataTypes;
std::unordered_map<MonoClass*, ScriptTypeInfo*> ScriptingCore::scriptsInfo;
std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> ScriptingCore::eventMethodsDescriptions;
MonoClass* ScriptingCore::baseScriptClass = nullptr;
ScriptComponentSystem* ScriptingCore::scriptComponentSystem;
Component nullComponent = Component(NULL_ENTITY);

void ScriptingCore::Init(MonoImage* image)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    scriptComponentSystem = new ScriptComponentSystem();
    registry->RegisterSystem<ScriptComponent>(scriptComponentSystem);

    LoadEventMethodsDescriptions(image);
    LoadEngineCallsMethods(image);
    LoadEventManagerMethods(image);
    LoadCoroutinesManagerMethods(image);
    RegisterInternalCalls();
    CacheAPITypes(image);
    CacheDataTypes(image);
}

void ScriptingCore::Terminate()
{
    for (auto desc : eventMethodsDescriptions)
        mono_method_desc_free(desc.second);
    for (auto info : scriptsInfo)
        delete info.second;
    delete cachedAPITypes;
    delete scriptComponentSystem;
}

void ScriptingCore::LoadEventMethodsDescriptions(MonoImage* image)
{
    baseScriptClass = mono_class_from_name(image, "Iron", "ScriptComponent");

    eventMethodsDescriptions[ScriptEventTypes::OnUpdate] = mono_method_desc_new("*:OnUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCreate] = mono_method_desc_new("*:OnCreate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnDestroy] = mono_method_desc_new("*:OnDestroy ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnFixedUpdate] = mono_method_desc_new("*:OnFixedUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnLateUpdate] = mono_method_desc_new("*:OnLateUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnEnabled] = mono_method_desc_new("*:OnEnabled ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnDisabled] = mono_method_desc_new("*:OnDisabled ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionEnter] = mono_method_desc_new("*:OnCollisionEnter (Iron.Collision)", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionStay] = mono_method_desc_new("*:OnCollisionStay (Iron.Collision)", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionExit] = mono_method_desc_new("*:OnCollisionExit (Iron.Collision)", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseOver] = mono_method_desc_new("*:OnMouseOver ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseEnter] = mono_method_desc_new("*:OnMouseEnter ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseExit] = mono_method_desc_new("*:OnMouseExit ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMousePressed] = mono_method_desc_new("*:OnMousePressed ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseJustPressed] = mono_method_desc_new("*:OnMouseJustPressed ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseJustReleased] = mono_method_desc_new("*:OnMouseJustReleased ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseOverUI] = mono_method_desc_new("*:OnMouseOverUI ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseEnterUI] = mono_method_desc_new("*:OnMouseEnterUI ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseExitUI] = mono_method_desc_new("*:OnMouseExitUI ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMousePressedUI] = mono_method_desc_new("*:OnMousePressedUI ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseJustPressedUI] = mono_method_desc_new("*:OnMouseJustPressedUI ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnMouseJustReleasedUI] = mono_method_desc_new("*:OnMouseJustReleasedUI ()", true);
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

    EngineCalls.callOnMouseOverUI = mono_class_get_method_from_name(klass, "ComponentOnMouseOverUI", 1);
    EngineCalls.callOnMouseEnterUI = mono_class_get_method_from_name(klass, "ComponentOnMouseEnterUI", 1);
    EngineCalls.callOnMouseExitUI = mono_class_get_method_from_name(klass, "ComponentOnMouseExitUI", 1);
    EngineCalls.callOnMousePressedUI = mono_class_get_method_from_name(klass, "ComponentOnMousePressedUI", 1);
    EngineCalls.callOnMouseJustPressedUI = mono_class_get_method_from_name(klass, "ComponentOnMouseJustPressedUI", 1);
    EngineCalls.callOnMouseJustReleasedUI = mono_class_get_method_from_name(klass, "ComponentOnMouseJustReleasedUI", 1);
}

void ScriptingCore::LoadEventManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, "Iron", "EventManager");

    EventManagerCalls.callInvokeCallbacks = mono_class_get_method_from_name(klass, "InvokeCallbacks", 2);
    EventManagerCalls.callDeregisterCallbacks = mono_class_get_method_from_name(klass, "DeregisterCallbacks", 2);
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
    CACHE_CLASS(UIInputField, API_CLASS(UIInputField))
    CACHE_CLASS(UIClipping, API_CLASS(UIClipping))
    CACHE_CLASS(UICheckBox, API_CLASS(UICheckBox))
    CACHE_CLASS(UITabs, API_CLASS(UITabs))
    CACHE_CLASS(UILayoutGroup, API_CLASS(UILayoutGroup))
}

void ScriptingCore::CacheDataTypes(MonoImage* image)
{
    cachedDataTypes.push_back(API_CLASS(RayCastHit));
    cachedDataTypes.push_back(API_CLASS(Color));
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
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return nullComponent;

    return ScriptingCore::AddComponentFromMonoClass(entity, monoClass, success);
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
    if (monoClass == CACHED_CLASS(UIInputField))
        return entitiesRegistry->AddComponent<UIInputField>(entity);
    if (monoClass == CACHED_CLASS(UIClipping))
        return entitiesRegistry->AddComponent<UIClipping>(entity);
    if (monoClass == CACHED_CLASS(UICheckBox))
        return entitiesRegistry->AddComponent<UICheckBox>(entity);
    if (monoClass == CACHED_CLASS(UITabs))
        return entitiesRegistry->AddComponent<UITabs>(entity);
    if (monoClass == CACHED_CLASS(UILayoutGroup))
        return entitiesRegistry->AddComponent<UILayoutGroup>(entity);

    Log::LogError("Could not find cached class");

    success = false;
    return nullComponent;
}

bool ScriptingCore::HasComponentFromType(EntityID entity, void* type, bool& success)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    success = true;
    return ScriptingCore::HasComponentFromMonoClass(entity, monoClass);
}

bool ScriptingCore::HasComponentFromMonoClass(EntityID entity, MonoClass* monoClass)
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
    if (monoClass == CACHED_CLASS(UIInputField))
        return entitiesRegistry->HasComponent<UIInputField>(entity);
    if (monoClass == CACHED_CLASS(UIClipping))
        return entitiesRegistry->HasComponent<UIClipping>(entity);
    if (monoClass == CACHED_CLASS(UICheckBox))
        return entitiesRegistry->HasComponent<UICheckBox>(entity);
    if (monoClass == CACHED_CLASS(UITabs))
        return entitiesRegistry->HasComponent<UITabs>(entity);
    if (monoClass == CACHED_CLASS(UILayoutGroup))
        return entitiesRegistry->HasComponent<UILayoutGroup>(entity);

    Log::LogError("Could not find cached class");

    return false;
}

bool ScriptingCore::RemoveComponentFromType(EntityID entity, void* type, bool& success)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return ScriptingCore::RemoveComponentFromMonoClass(entity, monoClass, success);
}

bool ScriptingCore::RemoveComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    success = true;
    
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
    if (monoClass == CACHED_CLASS(UIInputField))
        return entitiesRegistry->RemoveComponent<UIInputField>(entity);
    if (monoClass == CACHED_CLASS(UIClipping))
        return entitiesRegistry->RemoveComponent<UIClipping>(entity);
    if (monoClass == CACHED_CLASS(UICheckBox))
        return entitiesRegistry->RemoveComponent<UICheckBox>(entity);
    if (monoClass == CACHED_CLASS(UITabs))
        return entitiesRegistry->RemoveComponent<UITabs>(entity);
    if (monoClass == CACHED_CLASS(UILayoutGroup))
        return entitiesRegistry->RemoveComponent<UILayoutGroup>(entity);

    Log::LogError("Could not find cached class");

    success = false;
    return false;
}

bool ScriptingCore::ComponentOwnersFromType(void* type, std::vector<EntityID>& result)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

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
    if (monoClass == CACHED_CLASS(UIInputField))
        RETURN_COMPONENT_OWNERS(UIInputField)
    if (monoClass == CACHED_CLASS(UIClipping))
        RETURN_COMPONENT_OWNERS(UIClipping)
    if (monoClass == CACHED_CLASS(UICheckBox))
        RETURN_COMPONENT_OWNERS(UICheckBox)
    if (monoClass == CACHED_CLASS(UITabs))
        RETURN_COMPONENT_OWNERS(UITabs)
    if (monoClass == CACHED_CLASS(UILayoutGroup))
        RETURN_COMPONENT_OWNERS(UILayoutGroup)

    Log::LogError("Could not find cached class");

    return false;
}

bool ScriptingCore::AddScriptComponentFromType(EntityID entity, void* type, ScriptPointer scriptPointer)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return AddScriptComponentFromMonoClass(entity, monoClass, scriptPointer);
}

bool ScriptingCore::AddScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass, ScriptPointer scriptPointer)
{
    ScriptTypeInfo* typeInfo = ScriptParseRecursive(monoClass);

    auto& scriptComponent = AddComponentS<ScriptComponent>(entity);
    if (scriptComponent.HasScriptType(typeInfo))
        // We can support multiple script instances of the same type later if needed, for now return false
        return false;

    scriptComponent.AddScript(scriptPointer, typeInfo);

    return true;
}

bool ScriptingCore::HasScriptComponentFromType(EntityID entity, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

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
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return 0;

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
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

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
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

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

MonoClass* ScriptingCore::TypeToMonoClass(void* type)
{
    MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
    if (monoType == nullptr)
    {
        Log::LogError("Error getting mono type");
        return nullptr;
    }

    MonoClass* monoClass = mono_class_from_mono_type(monoType);
    if (monoClass == nullptr)
    {
        Log::LogError("Error converting mono type to mono class: {0}", mono_type_get_name(monoType));
        return nullptr;
    }

    return monoClass;
}

ScriptTypeInfo* ScriptingCore::ScriptParseRecursive(MonoClass* monoClass)
{
    if (scriptsInfo.find(monoClass) != scriptsInfo.end())
        return scriptsInfo[monoClass];

    auto mask = (ScriptEventTypes::ScriptEventType)0;

    MonoClass* parentClass = mono_class_get_parent(monoClass);
    if (parentClass != nullptr && parentClass != baseScriptClass)
        mask = mask | ScriptParseRecursive(parentClass)->Mask;

    for (auto desc : eventMethodsDescriptions)
    {
        auto method = mono_method_desc_search_in_class(desc.second, monoClass);
        if (method != nullptr)
            mask = mask | desc.first;
    }

    auto typeInfo = new ScriptTypeInfo();
    typeInfo->Mask = mask;
    scriptsInfo[monoClass] = typeInfo;

    return typeInfo;
}

MonoMethod* ScriptingCore::GetMethod(MonoImage* image, const char* methodName)
{
    MonoMethodDesc* description = mono_method_desc_new(methodName, NULL);
    if (!description)
    {
        Log::LogError("Unable to get method description {0}", methodName);
        return nullptr;
    }

    MonoMethod* method = mono_method_desc_search_in_image(description, image);
    if (!method)
    {
        Log::LogError("Unable to load method from image {0}", methodName);
        return nullptr;
    }

    return method;
}

void ScriptingCore::CallEventMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method)
{
    if (ownerEntityID == NULL_ENTITY)
    {
        Log::LogError("Trying to call method in null entity");
        return;
    }

    MonoObject* exception = nullptr;
    void* params[2];
    params[0] = &ownerEntityID;
    params[1] = &callbackType;
    mono_runtime_invoke(method, nullptr, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method {0}", mono_method_full_name(method, true));
        mono_print_unhandled_exception(exception);
    }
}

void ScriptingCore::CallMethod(MonoMethod* method)
{
    MonoObject* exception = nullptr;
    mono_runtime_invoke(method, nullptr, nullptr, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method {0}", mono_method_full_name(method, true));
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
        Log::LogError("Error calling method {0}", mono_method_full_name(method, true));
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
        Log::LogError("Error calling method {0}", mono_method_full_name(method, true));
        mono_print_unhandled_exception(exception);
    }
}

void ScriptingCore::FindAndCallEntryPoint(MonoImage* image)
{
    Log::LogDebug("FindAndCallEntryPoint");

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

MonoArray* ScriptingCore::ToMonoStringArray(const std::vector<std::string>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_string_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, MonoString*, i, mono_string_new(mono_domain_get(), inArray[i].c_str()));
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoFloatArray(const std::vector<float>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_single_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, float, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoIntArray(const std::vector<int>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_int32_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, int, i, inArray[i]);
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

void ScriptingCore::FromMonoStringArray(MonoArray* inArray, std::vector<std::string>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.emplace_back(mono_string_to_utf8(mono_array_get(inArray, MonoString*, i)));
    }
}

void ScriptingCore::FromMonoFloatArray(MonoArray* inArray, std::vector<float>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.emplace_back(mono_array_get(inArray, float, i));
    }
}

void ScriptingCore::FromMonoIntArray(MonoArray* inArray, std::vector<int>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.emplace_back(mono_array_get(inArray, int, i));
    }
}
