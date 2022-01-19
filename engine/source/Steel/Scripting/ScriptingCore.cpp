#include "ScriptingCore.h"
#include "ScriptComponent.h"
#include "ScriptingCallsRegister.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Application.h"

#define NAMESPACE_NAME "Steel"

EngineCallsMethods ScriptingCore::EngineCalls;
EventManagerMethods ScriptingCore::EventManagerCalls;
CoroutinesManagerMethods ScriptingCore::CoroutinesManagerCalls;
std::unordered_map<MonoClass*, const TypeInfo*> ScriptingCore::cachedAPITypes;
std::unordered_map<SimpleAPITypes::SimpleAPIType, MonoClass*> ScriptingCore::cachedSimpleAPITypes;
std::unordered_map<MonoClass*, ScriptTypeInfo*> ScriptingCore::scriptsInfo;
std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> ScriptingCore::eventMethodsDescriptions;
MonoClass* ScriptingCore::baseScriptClass = nullptr;

void ScriptingCore::Init(MonoImage* image)
{
    LoadEventMethodsDescriptions(image);
    LoadEngineCallsMethods(image);
    LoadEventManagerMethods(image);
    LoadCoroutinesManagerMethods(image);
    RegisterInternalCalls();
    CacheAPITypes(image);
    CacheSimpleAPITypes(image);
}

void ScriptingCore::Terminate()
{
    for (auto desc : eventMethodsDescriptions)
        mono_method_desc_free(desc.second);
    for (auto info : scriptsInfo)
        delete info.second;
}

void ScriptingCore::LoadEventMethodsDescriptions(MonoImage* image)
{
    baseScriptClass = mono_class_from_name(image, NAMESPACE_NAME, "ScriptComponent");

    eventMethodsDescriptions[ScriptEventTypes::OnUpdate] = mono_method_desc_new("*:OnUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCreate] = mono_method_desc_new("*:OnCreate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnDestroy] = mono_method_desc_new("*:OnDestroy ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnFixedUpdate] = mono_method_desc_new("*:OnFixedUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnLateUpdate] = mono_method_desc_new("*:OnLateUpdate ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnEnabled] = mono_method_desc_new("*:OnEnabled ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnDisabled] = mono_method_desc_new("*:OnDisabled ()", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionEnter] = mono_method_desc_new("*:OnCollisionEnter (Steel.Collision)", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionStay] = mono_method_desc_new("*:OnCollisionStay (Steel.Collision)", true);
    eventMethodsDescriptions[ScriptEventTypes::OnCollisionExit] = mono_method_desc_new("*:OnCollisionExit (Steel.Collision)", true);
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
    MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, "ComponentEngineCalls");

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
    MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, "EventManager");

    EventManagerCalls.callInvokeCallbacks = mono_class_get_method_from_name(klass, "InvokeCallbacks", 2);
    EventManagerCalls.callDeregisterCallbacks = mono_class_get_method_from_name(klass, "DeregisterCallbacks", 2);
}

void ScriptingCore::LoadCoroutinesManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, "CoroutinesManager");

    CoroutinesManagerCalls.callUpdate = mono_class_get_method_from_name(klass, "Update", 0);
}

void ScriptingCore::RegisterInternalCalls()
{
    ScriptingCallsRegister::RegisterInternalCalls();
}

void ScriptingCore::CacheAPITypes(MonoImage* image)
{
    std::vector<const TypeInfo*> typeInfos;
    TypeInfoStorage::GetTypeInfos(typeInfos);

    for (auto typeInfo : typeInfos)
    {
        MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, typeInfo->TypeName);
        if (klass == nullptr)
        {
            // It's ok if some types are not presented in scripting project
            continue;
        }

        cachedAPITypes[klass] = typeInfo;
    }
}

void ScriptingCore::CacheSimpleAPITypes(MonoImage* image)
{
    CACHE_SIMPLE_TYPE(Vector2);
    CACHE_SIMPLE_TYPE(Vector3);
    CACHE_SIMPLE_TYPE(Color);
    CACHE_SIMPLE_TYPE(RayCastHit);
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

bool ScriptingCore::AddComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return ScriptingCore::AddComponentFromMonoClass(entityID, monoClass);
}

bool ScriptingCore::AddComponentFromMonoClass(EntityID entityID, MonoClass* monoClass)
{
    if (cachedAPITypes.find(monoClass) == cachedAPITypes.end())
    {
        Log::LogError("Could not find cached class");

        return false;
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    return entitiesRegistry->AddComponent(entityID, cachedAPITypes[monoClass]->ID);
}

bool ScriptingCore::HasComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return ScriptingCore::HasComponentFromMonoClass(entityID, monoClass);
}

bool ScriptingCore::HasComponentFromMonoClass(EntityID entityID, MonoClass* monoClass)
{
    if (cachedAPITypes.find(monoClass) == cachedAPITypes.end())
    {
        Log::LogError("Could not find cached class");

        return false;
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    return entitiesRegistry->HasComponent(entityID, cachedAPITypes[monoClass]->ID);
}

bool ScriptingCore::RemoveComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return ScriptingCore::RemoveComponentFromMonoClass(entityID, monoClass);
}

bool ScriptingCore::RemoveComponentFromMonoClass(EntityID entityID, MonoClass* monoClass)
{
    if (cachedAPITypes.find(monoClass) == cachedAPITypes.end())
    {
        Log::LogError("Could not find cached class");

        return false;
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    return entitiesRegistry->RemoveComponent(entityID, cachedAPITypes[monoClass]->ID);
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
    if (cachedAPITypes.find(monoClass) == cachedAPITypes.end())
    {
        Log::LogError("Could not find cached class");

        return false;
    }

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    auto entitiesIterator = entitiesRegistry->GetEntitiesIterator(cachedAPITypes[monoClass]->ID, true);
    result.reserve(entitiesIterator.Size());

    for (int i = 0; i < entitiesIterator.Size(); ++i)
    {
        result.push_back(entitiesIterator[i]);
    }

    return true;
}

bool ScriptingCore::AddScriptComponentFromType(EntityID entityID, void* type, ScriptPointer scriptPointer)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return AddScriptComponentFromMonoClass(entityID, monoClass, scriptPointer);
}

bool ScriptingCore::AddScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass, ScriptPointer scriptPointer)
{
    ScriptTypeInfo* typeInfo = ScriptParseRecursive(monoClass);

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& scriptComponent = entitiesRegistry->AddComponent<ScriptComponent>(entityID);
    if (scriptComponent.HasScriptType(typeInfo))
        // We can support multiple script instances of the same type later if needed, for now return false
        return false;

    scriptComponent.AddScript(scriptPointer, typeInfo);

    return true;
}

bool ScriptingCore::HasScriptComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return HasScriptComponentFromMonoClass(entityID, monoClass);
}

bool ScriptingCore::HasScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass)
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

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entityID))
        return false;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entityID);

    return scriptComponent.HasScriptType(typeInfo);
}

ScriptPointer ScriptingCore::GetScriptComponentFromType(EntityID entityID, void* type, bool& success)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return 0;

    return GetScriptComponentFromMonoClass(entityID, monoClass, success);
}

ScriptPointer ScriptingCore::GetScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass, bool& success)
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

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entityID))
        return 0;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entityID);

    if (!scriptComponent.HasScriptType(typeInfo))
        return 0;

    success = true;
    return scriptComponent.GetScriptPointer(typeInfo);
}

bool ScriptingCore::RemoveScriptComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    return RemoveScriptComponentFromMonoClass(entityID, monoClass);
}

bool ScriptingCore::RemoveScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass)
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

    if (!entitiesRegistry->HasComponent<ScriptComponent>(entityID))
        return false;

    auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entityID);
    if (!scriptComponent.HasScriptType(typeInfo))
        return false;

    scriptComponent.RemoveScript(typeInfo);
    if (scriptComponent.Scripts.empty())
        entitiesRegistry->RemoveComponent<ScriptComponent>(entityID);

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
    MonoMethodDesc* description = mono_method_desc_new(methodName, 0);
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

    MonoClass* klass = mono_class_from_name(image, "SteelCustom", "GameManager");
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

const char* ScriptingCore::ToString(MonoString* monoString)
{
    const char* result = mono_string_to_utf8(monoString);
    return result ? result : "";
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

MonoArray* ScriptingCore::ToMonoUInt64Array(const std::vector<uint64_t>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_uint64_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, uint64_t, i, inArray[i]);
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

void ScriptingCore::FromMonoUInt64Array(MonoArray* inArray, std::vector<uint64_t>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.push_back(mono_array_get(inArray, uint64_t, i));
    }
}

void ScriptingCore::FromMonoStringArray(MonoArray* inArray, std::vector<std::string>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.emplace_back(ScriptingCore::ToString(mono_array_get(inArray, MonoString*, i)));
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
