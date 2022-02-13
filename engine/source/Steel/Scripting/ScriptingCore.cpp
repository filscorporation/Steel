#include "ScriptingCore.h"
#include "ScriptComponent.h"
#include "ScriptingCallsRegister.h"
#include "ScriptAttributeAccessor.h"
#include "ScriptMethodPointer.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Application.h"

#include <mono/metadata/attrdefs.h>

#define NAMESPACE_NAME "Steel"

EventManagerMethods ScriptingCore::EventManagerCalls;
CoroutinesManagerMethods ScriptingCore::CoroutinesManagerCalls;
std::unordered_map<MonoClass*, const TypeInfo*> ScriptingCore::cachedAPITypes;
std::unordered_map<SimpleAPITypes::SimpleAPIType, MonoClass*> ScriptingCore::cachedSimpleAPITypes;
std::unordered_map<APIStructs::APIStruct, std::pair<MonoClass*, MonoMethod*>> ScriptingCore::cachedAPIStructs;
std::unordered_map<MonoClass*, ScriptTypeInfo*> ScriptingCore::scriptsInfo;
std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> ScriptingCore::eventMethodsDescriptions;
MonoClass* ScriptingCore::baseScriptClass = nullptr;
MonoClass* ScriptingCore::serializableAttributeClass = nullptr;
MonoClass* ScriptingCore::componentClass = nullptr;
MonoClass* ScriptingCore::entityClass = nullptr;
MonoMethod* ScriptingCore::entityConstructorMethod = nullptr;
MonoProperty* ScriptingCore::componentEntityProperty = nullptr;

void ScriptingCore::Init(MonoImage* image)
{
    LoadSpecialTypes(image);
    LoadEventMethodsDescriptions(image);
    LoadEventManagerMethods(image);
    LoadCoroutinesManagerMethods(image);
    RegisterInternalCalls();
    CacheAPITypes(image);
    CacheSimpleAPITypes(image);
    CacheAPIStructs(image);
}

void ScriptingCore::Terminate()
{
    for (auto desc : eventMethodsDescriptions)
        mono_method_desc_free(desc.second);
    for (auto info : scriptsInfo)
    {
        for (auto& fieldInfo : info.second->Attributes)
            delete fieldInfo.Accessor;
        for (auto& methodInfoPair : info.second->EventMethods)
            delete methodInfoPair.second;

        delete info.second;
    }
}

void ScriptingCore::LoadSpecialTypes(MonoImage* image)
{
    baseScriptClass = mono_class_from_name(image, NAMESPACE_NAME, "ScriptComponent");
    serializableAttributeClass = mono_class_from_name(image, NAMESPACE_NAME, "SerializeFieldAttribute");

    componentClass = mono_class_from_name(image, NAMESPACE_NAME, "Component");
    entityClass = mono_class_from_name(image, NAMESPACE_NAME, "Entity");
    entityConstructorMethod = mono_class_get_method_from_name_flags(entityClass, ".ctor", 1, MONO_METHOD_ATTR_ASSEM);

    componentEntityProperty = mono_class_get_property_from_name(componentClass, "Entity");
}

void ScriptingCore::LoadEventMethodsDescriptions(MonoImage* image)
{
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

void ScriptingCore::CacheAPIStructs(MonoImage* image)
{
    // TODO: refactor to cache internal types in one place and fill all needed data in struct: class, ctors, etc

    // Collision
    MonoClass* monoClass = mono_class_from_name(image, "Steel", "Collision");
    auto pair = std::pair(monoClass, mono_class_get_method_from_name(monoClass, ".ctor", 1));
    cachedAPIStructs[APIStructs::Collision] = pair;
}

MonoObject* ScriptingCore::AddComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return nullptr;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (cachedAPITypes.find(monoClass) != cachedAPITypes.end())
    {
        // Built in types
        if (!entitiesRegistry->AddComponent(entityID, cachedAPITypes[monoClass]->ID))
            return nullptr;

        MonoObject* monoObject = CreateUnmanagedInstance(monoClass, nullptr, 0);
        SetEntityOwner(monoObject, entityID);

        return monoObject;
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo = ScriptParseRecursive(monoClass);
        auto& scriptComponent = entitiesRegistry->AddComponent<ScriptComponent>(entityID);
        if (scriptComponent.HasScriptType(typeInfo))
            // We can support multiple script instances of the same type later if needed, for now return existing
            return scriptComponent.GetScriptPointer(typeInfo)->GetMonoObject();

        ScriptPointer* pointer = CreateManagedInstance(monoClass, nullptr, 0);
        if (pointer == nullptr)
            return nullptr;

        SetEntityOwner(pointer->GetMonoObject(), entityID);

        scriptComponent.AddScript(pointer, typeInfo);

        return pointer->GetMonoObject();
    }

    Log::LogError("Type is not internal or subclass of ScriptComponent");

    return nullptr;
}

bool ScriptingCore::HasComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (cachedAPITypes.find(monoClass) != cachedAPITypes.end())
    {
        // Built in types
        return entitiesRegistry->HasComponent(entityID, cachedAPITypes[monoClass]->ID);
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo;
        if (scriptsInfo.find(monoClass) == scriptsInfo.end())
            return false;
        else
            typeInfo = scriptsInfo[monoClass];

        if (!entitiesRegistry->HasComponent<ScriptComponent>(entityID))
            return false;

        auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entityID);

        return scriptComponent.HasScriptType(typeInfo);
    }

    Log::LogError("Type is not internal or subclass of ScriptComponent");

    return false;
}

MonoObject* ScriptingCore::GetComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return nullptr;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (cachedAPITypes.find(monoClass) != cachedAPITypes.end())
    {
        // Built in types
        if (!entitiesRegistry->HasComponent(entityID, cachedAPITypes[monoClass]->ID))
            return nullptr;

        MonoObject* monoObject = CreateUnmanagedInstance(monoClass, nullptr, 0);
        SetEntityOwner(monoObject, entityID);

        return monoObject;
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo = ScriptParseRecursive(monoClass);

        if (!entitiesRegistry->HasComponent<ScriptComponent>(entityID))
            return nullptr;

        auto& scriptComponent = entitiesRegistry->GetComponent<ScriptComponent>(entityID);
        if (scriptComponent.HasScriptType(typeInfo))
            return scriptComponent.GetScriptPointer(typeInfo)->GetMonoObject();
        else
            return nullptr;
    }

    Log::LogError("Type is not internal or subclass of ScriptComponent");

    return nullptr;
}

bool ScriptingCore::RemoveComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return false;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (cachedAPITypes.find(monoClass) != cachedAPITypes.end())
    {
        // Built in types
        return entitiesRegistry->RemoveComponent(entityID, cachedAPITypes[monoClass]->ID);
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo;
        if (scriptsInfo.find(monoClass) == scriptsInfo.end())
        {
            return false;
        }
        else
        {
            typeInfo = scriptsInfo[monoClass];
        }

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

    Log::LogError("Type is not internal or subclass of ScriptComponent");

    return false;
}

void ScriptingCore::ComponentOwnersFromType(void* type, MonoObject** result)
{
    // TODO: refactor returns
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
    {
        *result = (MonoObject*)mono_array_new(mono_domain_get(), mono_get_object_class(), 0);
        return;
    }

    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (cachedAPITypes.find(monoClass) != cachedAPITypes.end())
    {
        // Built in types
        auto entitiesIterator = entitiesRegistry->GetEntitiesIterator(cachedAPITypes[monoClass]->ID, true);
        MonoArray* resultArray = mono_array_new(mono_domain_get(), mono_get_object_class(), entitiesIterator.Size());

        for (int i = 0; i < entitiesIterator.Size(); ++i)
        {
            MonoObject* monoObject = CreateUnmanagedInstance(monoClass, nullptr, 0);
            SetEntityOwner(monoObject, entitiesIterator[i]);
            mono_array_set(resultArray, MonoObject*, i, monoObject);
        }

        *result = (MonoObject*)resultArray;
        return;
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo;
        if (scriptsInfo.find(monoClass) == scriptsInfo.end())
        {
            *result = (MonoObject*)mono_array_new(mono_domain_get(), mono_get_object_class(), 0);
            return;
        }
        typeInfo = scriptsInfo[monoClass];

        auto iterator = entitiesRegistry->GetComponentIterator<ScriptComponent>();

        int count = 0;
        for (auto& script : iterator)
        {
            if (script.HasScriptType(typeInfo))
                count++;
        }
        MonoArray* resultArray = mono_array_new(mono_domain_get(), mono_get_object_class(), count);
        int i = 0;
        for (auto& script : iterator)
        {
            if (script.HasScriptType(typeInfo))
            {
                mono_array_set(resultArray, MonoObject*, i, script.GetScriptPointer(typeInfo)->GetMonoObject());
                i++;
            }
        }

        *result = (MonoObject*)resultArray;
        return;
    }

    Log::LogError("Type is not internal or subclass of ScriptComponent");

    *result = (MonoObject*)mono_array_new(mono_domain_get(), mono_get_object_class(), 0);
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

bool ScriptingCore::IsSubclassOfScriptComponent(MonoClass* monoClass)
{
    if (scriptsInfo.find(monoClass) != scriptsInfo.end())
        return true;

    MonoClass* parentClass = mono_class_get_parent(monoClass);
    while (parentClass != nullptr)
        if (parentClass == baseScriptClass)
            return true;

    return false;
}

ScriptPointer* ScriptingCore::CreateManagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount)
{
    MonoObject* monoObject = CreateUnmanagedInstance(monoClass, constructorParams, paramsCount);

    if (monoObject == nullptr)
        return nullptr;

    return new ScriptPointer(monoObject);
}

MonoObject* ScriptingCore::CreateUnmanagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount)
{
    MonoObject* monoObject = mono_object_new(mono_domain_get(), monoClass);

    if (constructorParams != nullptr)
    {
        MonoMethod* constructorMethod = mono_class_get_method_from_name(monoClass, ".ctor", paramsCount);

        if (!CallMethod(constructorMethod, monoObject, constructorParams))
            return nullptr;
    }
    else
    {
        // Execute default constructor
        mono_runtime_object_init(monoObject);
    }

    return monoObject;
}

ScriptPointer* ScriptingCore::CreateAPIStruct(APIStructs::APIStruct structType, void** constructorParams)
{
    MonoObject* monoObject = mono_object_new(mono_domain_get(), cachedAPIStructs[structType].first);
    auto pointer = new ScriptPointer(monoObject);

    if (constructorParams != nullptr)
    {
        if (!CallMethod(cachedAPIStructs[structType].second, monoObject, constructorParams))
            return nullptr;
    }
    else
    {
        // Execute default constructor
        mono_runtime_object_init(monoObject);
    }

    return pointer;
}

ScriptTypeInfo* ScriptingCore::ScriptParseRecursive(MonoClass* monoClass)
{
    if (scriptsInfo.find(monoClass) != scriptsInfo.end())
        return scriptsInfo[monoClass];

    auto mask = (ScriptEventTypes::ScriptEventType)0;

    auto typeInfo = new ScriptTypeInfo();
    scriptsInfo[monoClass] = typeInfo;

    typeInfo->TypeName = mono_class_get_name(monoClass);
    typeInfo->TypeNamespace = mono_class_get_namespace(monoClass);

    void* iter = nullptr;
    MonoClassField* monoField;
    while ((monoField = mono_class_get_fields(monoClass, &iter)) != nullptr)
    {
        if (!CanSerializeField(monoClass, monoField))
            continue;

        ScriptAttributeInfo fieldInfo;
        fieldInfo.FieldName = mono_field_get_name(monoField);
        MonoType* type = mono_field_get_type(monoField);
        fieldInfo.Accessor = CreateFieldAccessor(monoField, type);

        typeInfo->Attributes.emplace_back(fieldInfo);
    }

    MonoClass* parentClass = mono_class_get_parent(monoClass);
    if (parentClass != nullptr && parentClass != baseScriptClass)
    {
        ScriptTypeInfo* parentTypeInfo = ScriptParseRecursive(parentClass);
        for (auto pair : parentTypeInfo->EventMethods)
        {
            typeInfo->EventMethods[pair.first] = new ScriptMethodPointer(pair.second->Method, true);
        }
        mask = mask | parentTypeInfo->Mask;
    }

    for (auto desc : eventMethodsDescriptions)
    {
        MonoMethod* method = mono_method_desc_search_in_class(desc.second, monoClass);
        if (method != nullptr)
            mask = mask | desc.first;
        typeInfo->EventMethods[desc.first] = new ScriptMethodPointer(method, false);
    }
    typeInfo->Mask = mask;

    return typeInfo;
}

void ScriptingCore::SetEntityOwner(MonoObject* monoObject, EntityID entityID)
{
    MonoObject* entityObject = mono_object_new(mono_domain_get(), entityClass);

    void* constructorParams[1];
    constructorParams[0] = &entityID;
    if (!CallMethod(entityConstructorMethod, entityObject, constructorParams))
        return;

    MonoObject* exception = nullptr;
    void* propertyParams[1];
    propertyParams[0] = entityObject;
    mono_property_set_value(componentEntityProperty, monoObject, propertyParams, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error creating entity");
        mono_print_unhandled_exception(exception);
    }
}

bool ScriptingCore::CanSerializeField(MonoClass* monoClass, MonoClassField* monoClassField)
{
    auto attributes = mono_custom_attrs_from_field(monoClass, monoClassField);
    bool result = attributes != nullptr && mono_custom_attrs_has_attr(attributes, serializableAttributeClass);
    mono_custom_attrs_free(attributes);
    return result;
}

ScriptAttributeAccessorBase* ScriptingCore::CreateFieldAccessor(MonoClassField* monoClassField, MonoType* monoType)
{
    // TODO: finish missing types
    auto monoTypeEnum = mono_type_get_type(monoType);
    switch (monoTypeEnum)
    {
        case MONO_TYPE_BOOLEAN:
            return new ScriptAttributeAccessor<bool>(monoClassField, Types::Bool);
        case MONO_TYPE_CHAR:
            return new ScriptAttributeAccessor<char>(monoClassField, Types::Char);
        case MONO_TYPE_I1:
            return nullptr; // int8_t
        case MONO_TYPE_I2:
            return nullptr; // int16_t
        case MONO_TYPE_I4:
            return new ScriptAttributeAccessor<int>(monoClassField, Types::Int);
        case MONO_TYPE_I8:
            return new ScriptAttributeAccessor<long>(monoClassField, Types::Long);
        case MONO_TYPE_U1:
            return nullptr; // uint8_t
        case MONO_TYPE_U2:
            return nullptr; // uint16_t
        case MONO_TYPE_U4:
            return new ScriptAttributeAccessor<uint32_t>(monoClassField, Types::UInt32);
        case MONO_TYPE_U8:
            return new ScriptAttributeAccessor<uint64_t>(monoClassField, Types::UInt64);
        case MONO_TYPE_R4:
            return new ScriptAttributeAccessor<float>(monoClassField, Types::Float);
        case MONO_TYPE_R8:
            return new ScriptAttributeAccessor<double>(monoClassField, Types::Double);
        case MONO_TYPE_VALUETYPE:
            {
                Log::LogWarning("Value types serialization is not supported yet");
                //MonoClass* monoClass = mono_class_from_mono_type(monoType);

                return nullptr;
            }
        case MONO_TYPE_STRING:
            return new ScriptAttributeAccessor<std::string>(monoClassField, Types::String); // TODO: handle MonoString*
        case MONO_TYPE_ARRAY:
        case MONO_TYPE_SZARRAY:
            Log::LogWarning("Array types serialization is not supported yet");
            return nullptr;
        case MONO_TYPE_GENERICINST:
            Log::LogWarning("Generic types serialization is not supported yet");
            return nullptr;
        case MONO_TYPE_OBJECT:
            Log::LogWarning("Objects serialization is not supported yet");
            return nullptr;
        default:
            Log::LogWarning("Unknown field type");
            return nullptr;
    }
}

bool ScriptingCore::CallMethod(MonoMethod* method, MonoObject* monoObject, void** params)
{
    MonoObject* exception = nullptr;
    mono_runtime_invoke(method, monoObject, params, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error calling method {0}", mono_method_full_name(method, true));
        mono_print_unhandled_exception(exception);

        return false;
    }

    return true;
}

void ScriptingCore::CallEventMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method)
{
    void* params[2];
    params[0] = &ownerEntityID;
    params[1] = &callbackType;
    CallMethod(method, nullptr, params);
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

MonoArray* ScriptingCore::ToMonoObjectArray(const std::vector<MonoObject*>& inArray)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), mono_get_object_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, MonoObject*, i, inArray[i]);
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

void ScriptingCore::FromMonoObjectArray(MonoArray* inArray, std::vector<MonoObject*>& outArray)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.emplace_back(mono_array_get(inArray, MonoObject*, i));
    }
}
