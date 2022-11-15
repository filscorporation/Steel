#include "ScriptingCore.h"
#include "ScriptComponent.h"
#include "ScriptingCallsRegister.h"
#include "Steel/Scripting/Accessors/ScriptAttributeAccessor.h"
#include "ScriptMethodPointer.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Application.h"
#include "Steel/Scripting/Accessors/ScriptStringAttributeAccessor.h"
#include "Steel/EntityComponentSystem/TypeInfoStorage.h"

#include <mono/metadata/attrdefs.h>

#define NAMESPACE_NAME "Steel"
#define ROOT_DOMAIN_NAME "SteelRootDomain"
#define DOMAIN_NAME "SteelDomain"

ScriptingDomain* ScriptingCore::Domain = nullptr;

#if defined(_WIN32) || defined(_WIN64)
int setenv(const char* name, const char* value, int overwrite)
{
    int errcode = 0;
    if (!overwrite)
    {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, nullptr, 0, name);
        if (errcode || envsize)
            return errcode;
    }

    return _putenv_s(name, value);
}
#endif

bool ScriptingCore::Init(const char* monoLibPath, const char* monoEtcPath)
{
    mono_config_parse(nullptr);
    mono_set_dirs(monoLibPath, monoEtcPath);
    setenv("MONO_THREADS_SUSPEND", "preemptive", 1);

    MonoDomain* domain = mono_jit_init(ROOT_DOMAIN_NAME);

    return domain != nullptr;
}

void ScriptingCore::Terminate()
{
    MonoDomain* domain = mono_get_root_domain();
    if (domain != nullptr)
        mono_jit_cleanup(domain);
}

inline bool FileExists(const char* fileName)
{
    if (FILE *file = fopen(fileName, "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

bool ScriptingCore::CreateDomain(const char* apiDllPath, const char* scriptsDllPath)
{
    Domain = new ScriptingDomain();

    Domain->Domain = mono_domain_create_appdomain(const_cast<char*>(DOMAIN_NAME), nullptr);
    mono_domain_set(Domain->Domain, true);

    Domain->CoreAssemblyImage = LoadAssemblyImage(apiDllPath);
    if (Domain->CoreAssemblyImage == nullptr)
    {
        delete Domain;
        Domain = nullptr;
        return false;
    }

    Domain->CustomAssemblyImage = LoadAssemblyImage(scriptsDllPath);
    if (Domain->CustomAssemblyImage == nullptr)
    {
        delete Domain;
        Domain = nullptr;
        return false;
    }

    LoadSpecialTypes(Domain->CoreAssemblyImage);
    LoadEventMethodsDescriptions(Domain->CoreAssemblyImage);
    LoadEventManagerMethods(Domain->CoreAssemblyImage);
    LoadCoroutinesManagerMethods(Domain->CoreAssemblyImage);
    RegisterInternalCalls();
    CacheAPITypes(Domain->CoreAssemblyImage);
    CacheSimpleAPITypes(Domain->CoreAssemblyImage);
    CacheAPIStructs(Domain->CoreAssemblyImage);

    Log::LogDebug("Domain created");

    return true;
}

void ScriptingCore::UnloadDomain()
{
    if (Domain == nullptr)
        return;

    for (auto desc : Domain->EventMethodsDescriptions)
        mono_method_desc_free(desc.second);
    for (auto info : Domain->ScriptsInfo)
    {
        for (auto& fieldInfo : info.second->Attributes)
            delete fieldInfo.Accessor;
        for (auto& methodInfoPair : info.second->EventMethods)
            delete methodInfoPair.second;

        delete info.second;
    }

    if (Domain->Domain == mono_domain_get())
        mono_domain_set(mono_get_root_domain(), true);

    mono_domain_finalize(Domain->Domain, 2000);

    MonoObject* exception = nullptr;
    mono_domain_try_unload(Domain->Domain, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error unloading domain");
        mono_print_unhandled_exception(exception);
    }

    delete Domain;
    Domain = nullptr;

    Log::LogDebug("Domain unloaded");
}

bool ScriptingCore::DomainLoaded()
{
    return Domain != nullptr;
}

MonoImage* ScriptingCore::LoadAssemblyImage(const char* filePath)
{
    if (!FileExists(filePath))
    {
        Log::LogError("Error loading assembly, file does not exist: {0}", filePath);
        return nullptr;
    }

    MonoAssembly* assembly = mono_domain_assembly_open(Domain->Domain, filePath);
    if (assembly == nullptr)
    {
        Log::LogError("Error loading assembly {0}", filePath);
        return nullptr;
    }
    MonoImage* assemblyImage = mono_assembly_get_image(assembly);
    if (assemblyImage == nullptr)
    {
        Log::LogError("Error creating image for assembly {0}", filePath);
        return nullptr;
    }

    return assemblyImage;
}

void ScriptingCore::LoadSpecialTypes(MonoImage* image)
{
    Domain->BaseScriptClass = mono_class_from_name(image, NAMESPACE_NAME, "ScriptComponent");
    Domain->SerializableAttributeClass = mono_class_from_name(image, NAMESPACE_NAME, "SerializeFieldAttribute");

    Domain->ComponentClass = mono_class_from_name(image, NAMESPACE_NAME, "Component");
    Domain->EntityClass = mono_class_from_name(image, NAMESPACE_NAME, "Entity");
    Domain->EntityConstructorMethod = mono_class_get_method_from_name_flags(Domain->EntityClass, ".ctor", 1, MONO_METHOD_ATTR_ASSEM);

    Domain->ComponentEntityProperty = mono_class_get_property_from_name(Domain->ComponentClass, "Entity");
}

void ScriptingCore::LoadEventMethodsDescriptions(MonoImage* image)
{
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnUpdate] = mono_method_desc_new("*:OnUpdate ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnCreate] = mono_method_desc_new("*:OnCreate ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnDestroy] = mono_method_desc_new("*:OnDestroy ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnFixedUpdate] = mono_method_desc_new("*:OnFixedUpdate ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnLateUpdate] = mono_method_desc_new("*:OnLateUpdate ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnEnabled] = mono_method_desc_new("*:OnEnabled ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnDisabled] = mono_method_desc_new("*:OnDisabled ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnCollisionEnter] = mono_method_desc_new("*:OnCollisionEnter (Steel.Collision)", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnCollisionStay] = mono_method_desc_new("*:OnCollisionStay (Steel.Collision)", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnCollisionExit] = mono_method_desc_new("*:OnCollisionExit (Steel.Collision)", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseOver] = mono_method_desc_new("*:OnMouseOver ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseEnter] = mono_method_desc_new("*:OnMouseEnter ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseExit] = mono_method_desc_new("*:OnMouseExit ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMousePressed] = mono_method_desc_new("*:OnMousePressed ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseJustPressed] = mono_method_desc_new("*:OnMouseJustPressed ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseJustReleased] = mono_method_desc_new("*:OnMouseJustReleased ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseOverUI] = mono_method_desc_new("*:OnMouseOverUI ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseEnterUI] = mono_method_desc_new("*:OnMouseEnterUI ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseExitUI] = mono_method_desc_new("*:OnMouseExitUI ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMousePressedUI] = mono_method_desc_new("*:OnMousePressedUI ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseJustPressedUI] = mono_method_desc_new("*:OnMouseJustPressedUI ()", true);
    Domain->EventMethodsDescriptions[ScriptEventTypes::OnMouseJustReleasedUI] = mono_method_desc_new("*:OnMouseJustReleasedUI ()", true);
}

void ScriptingCore::LoadEventManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, "EventManager");

    Domain->InvokeCallbacksMethod = mono_class_get_method_from_name(klass, "InvokeCallbacks", 2);
    Domain->DeregisterCallbacksMethod = mono_class_get_method_from_name(klass, "DeregisterCallbacks", 2);
}

void ScriptingCore::LoadCoroutinesManagerMethods(MonoImage* image)
{
    MonoClass* klass = mono_class_from_name(image, NAMESPACE_NAME, "CoroutinesManager");

    Domain->CoroutinesUpdateMethod = mono_class_get_method_from_name(klass, "Update", 0);
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

        Domain->CachedAPITypes[klass] = typeInfo;
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
    Domain->CachedAPIStructs[APIStructs::Collision] = pair;
}

MonoObject* ScriptingCore::AddComponentFromType(EntityID entityID, void* type)
{
    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass == nullptr)
        return nullptr;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (Domain->CachedAPITypes.find(monoClass) != Domain->CachedAPITypes.end())
    {
        // Built in types
        if (!entitiesRegistry->AddComponent(entityID, Domain->CachedAPITypes[monoClass]->ID))
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
            return scriptComponent.GetScriptHandler(typeInfo)->GetMonoObject();

        ScriptObjectHandler* handler = CreateManagedInstance(monoClass, nullptr, 0);
        if (handler == nullptr)
            return nullptr;

        SetEntityOwner(handler->GetMonoObject(), entityID);

        scriptComponent.AddScript(handler, typeInfo);

        return handler->GetMonoObject();
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
    if (Domain->CachedAPITypes.find(monoClass) != Domain->CachedAPITypes.end())
    {
        // Built in types
        return entitiesRegistry->HasComponent(entityID, Domain->CachedAPITypes[monoClass]->ID);
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo;
        if (Domain->ScriptsInfo.find(monoClass) == Domain->ScriptsInfo.end())
            return false;
        else
            typeInfo = Domain->ScriptsInfo[monoClass];

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
    if (Domain->CachedAPITypes.find(monoClass) != Domain->CachedAPITypes.end())
    {
        // Built in types
        if (!entitiesRegistry->HasComponent(entityID, Domain->CachedAPITypes[monoClass]->ID))
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
            return scriptComponent.GetScriptHandler(typeInfo)->GetMonoObject();
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
    if (Domain->CachedAPITypes.find(monoClass) != Domain->CachedAPITypes.end())
    {
        // Built in types
        return entitiesRegistry->RemoveComponent(entityID, Domain->CachedAPITypes[monoClass]->ID);
    }

    if (IsSubclassOfScriptComponent(monoClass))
    {
        // Custom script types
        ScriptTypeInfo* typeInfo;
        if (Domain->ScriptsInfo.find(monoClass) == Domain->ScriptsInfo.end())
            return false;
        else
            typeInfo = Domain->ScriptsInfo[monoClass];

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

void ScriptingCore::GetComponentsListFromType(void* type, bool includeInactive, MonoObject** result)
{
    *result = nullptr;

    MonoClass* monoClass = TypeToMonoClass(type);
    if (monoClass != nullptr)
    {
        if (Domain->CachedAPITypes.find(monoClass) != Domain->CachedAPITypes.end())
        {
            // Built in types
            *result = (MonoObject*)GetBuiltInComponentsList(monoClass, includeInactive);
        }
        else if (IsSubclassOfScriptComponent(monoClass))
        {
            // Custom script types
            *result = (MonoObject*)GetCustomComponentsList(monoClass, includeInactive);
        }
        else
            Log::LogError("Type is not internal or subclass of ScriptComponent");
    }

    if (*result == nullptr)
        *result = (MonoObject*)mono_array_new(Domain->Domain, mono_get_object_class(), 0);
}

MonoArray* ScriptingCore::GetBuiltInComponentsList(MonoClass* monoClass, bool includeInactive)
{
    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    std::vector<EntitiesIterator> iterators;
    iterators.emplace_back(entitiesRegistry->GetEntitiesIterator(Domain->CachedAPITypes[monoClass]->ID, true));
    if (includeInactive)
        iterators.emplace_back(entitiesRegistry->GetEntitiesIterator(Domain->CachedAPITypes[monoClass]->ID, false));

    int size = 0;
    for (auto& iterator : iterators)
        // Size of result components list will exclude removed elements
        size += iterator.CondensedSize();

    MonoArray* resultArray = mono_array_new(Domain->Domain, mono_get_object_class(), size);

    for (auto& iterator : iterators)
    {
        int realCount = 0;
        // For iteration, we are using size including removed element, but skip them for final result
        for (int i = 0; i < iterator.Size(); ++i)
        {
            if (iterator[i] == NULL_ENTITY)
                // Skip null elements
                continue;

            MonoObject* monoObject = CreateUnmanagedInstance(monoClass, nullptr, 0);
            SetEntityOwner(monoObject, iterator[i]);
            mono_array_set(resultArray, MonoObject*, realCount, monoObject);
            realCount++;
        }
    }

    return resultArray;
}

MonoArray* ScriptingCore::GetCustomComponentsList(MonoClass* monoClass, bool includeInactive)
{
    EntitiesRegistry* entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (Domain->ScriptsInfo.find(monoClass) != Domain->ScriptsInfo.end())
    {
        ScriptTypeInfo* typeInfo = Domain->ScriptsInfo[monoClass];

        std::vector<ComponentIterator<ScriptComponent>> iterators;
        iterators.emplace_back(entitiesRegistry->GetComponentIterator<ScriptComponent>(true));
        if (includeInactive)
            iterators.emplace_back(entitiesRegistry->GetComponentIterator<ScriptComponent>(false));

        int count = 0;

        for (auto& iterator : iterators)
        {
            for (auto& script : iterator)
            {
                if (script.IsAlive() && script.HasScriptType(typeInfo))
                    count++;
            }
        }

        MonoArray* resultArray = mono_array_new(Domain->Domain, mono_get_object_class(), count);

        int i = 0;

        for (auto& iterator : iterators)
        {
            for (auto& script : iterator)
            {
                if (script.IsAlive() && script.HasScriptType(typeInfo))
                {
                    mono_array_set(resultArray, MonoObject*, i, script.GetScriptHandler(typeInfo)->GetMonoObject());
                    i++;
                }
            }
        }

        return resultArray;
    }

    return nullptr;
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

MonoClass* ScriptingCore::GetMonoClassByFullName(const std::string& classNamespace, const std::string& className)
{
    return mono_class_from_name(Domain->CustomAssemblyImage, classNamespace.c_str(), className.c_str());
}

bool ScriptingCore::IsSubclassOfScriptComponent(MonoClass* monoClass)
{
    if (Domain->ScriptsInfo.find(monoClass) != Domain->ScriptsInfo.end())
        return true;

    MonoClass* parentClass = mono_class_get_parent(monoClass);
    while (parentClass != nullptr)
    {
        if (parentClass == Domain->BaseScriptClass)
            return true;
        parentClass = mono_class_get_parent(parentClass);
    }

    return false;
}

ScriptObjectHandler* ScriptingCore::CreateManagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount)
{
    MonoObject* monoObject = CreateUnmanagedInstance(monoClass, constructorParams, paramsCount);

    if (monoObject == nullptr)
        return nullptr;

    return new ScriptObjectHandler(monoObject);
}

MonoObject* ScriptingCore::CreateUnmanagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount)
{
    MonoObject* monoObject = mono_object_new(Domain->Domain, monoClass);

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

MonoObject* ScriptingCore::CreateAPIStruct(APIStructs::APIStruct structType, void** constructorParams)
{
    MonoObject* monoObject = mono_object_new(Domain->Domain, Domain->CachedAPIStructs[structType].first);

    if (constructorParams != nullptr)
    {
        if (!CallMethod(Domain->CachedAPIStructs[structType].second, monoObject, constructorParams))
            return nullptr;
    }
    else
    {
        // Execute default constructor
        mono_runtime_object_init(monoObject);
    }

    return monoObject;
}

ScriptTypeInfo* ScriptingCore::ScriptParseRecursive(MonoClass* monoClass)
{
    if (Domain->ScriptsInfo.find(monoClass) != Domain->ScriptsInfo.end())
        return Domain->ScriptsInfo[monoClass];

    auto mask = (ScriptEventTypes::ScriptEventType)0;

    auto typeInfo = new ScriptTypeInfo();
    Domain->ScriptsInfo[monoClass] = typeInfo;

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
    if (parentClass != nullptr && parentClass != Domain->BaseScriptClass)
    {
        ScriptTypeInfo* parentTypeInfo = ScriptParseRecursive(parentClass);
        for (auto pair : parentTypeInfo->EventMethods)
        {
            typeInfo->EventMethods[pair.first] = new ScriptMethodPointer(pair.second->Method, true);
        }
        mask = mask | parentTypeInfo->Mask;
    }

    for (auto desc : Domain->EventMethodsDescriptions)
    {
        MonoMethod* method = mono_method_desc_search_in_class(desc.second, monoClass);
        if (method != nullptr)
        {
            mask = mask | desc.first;
            typeInfo->EventMethods[desc.first] = new ScriptMethodPointer(method, false);
        }
    }
    typeInfo->Mask = mask;

    return typeInfo;
}

void ScriptingCore::SetEntityOwner(MonoObject* monoObject, EntityID entityID)
{
    MonoObject* entityObject = CreateEntityObject(entityID);
    if (entityObject == nullptr)
        return;

    MonoObject* exception = nullptr;
    void* propertyParams[1];
    propertyParams[0] = entityObject;
    mono_property_set_value(Domain->ComponentEntityProperty, monoObject, propertyParams, &exception);

    if (exception != nullptr)
    {
        Log::LogError("Error setting owner entity field");
        mono_print_unhandled_exception(exception);
    }
}

MonoObject* ScriptingCore::CreateEntityObject(EntityID entityID)
{
    MonoObject* entityObject = mono_object_new(Domain->Domain, Domain->EntityClass);

    void* constructorParams[1];
    constructorParams[0] = &entityID;
    if (!CallMethod(Domain->EntityConstructorMethod, entityObject, constructorParams))
        return nullptr;

    return entityObject;
}

bool ScriptingCore::CanSerializeField(MonoClass* monoClass, MonoClassField* monoClassField)
{
    auto attributes = mono_custom_attrs_from_field(monoClass, monoClassField);
    bool result = attributes != nullptr && mono_custom_attrs_has_attr(attributes, Domain->SerializableAttributeClass);
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
            Log::LogWarning("Int8 serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr; // int8_t
        case MONO_TYPE_I2:
            Log::LogWarning("Int16 serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr; // int16_t
        case MONO_TYPE_I4:
            return new ScriptAttributeAccessor<int>(monoClassField, Types::Int);
        case MONO_TYPE_I8:
            return new ScriptAttributeAccessor<long>(monoClassField, Types::Long);
        case MONO_TYPE_U1:
            Log::LogWarning("UInt8 serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr; // uint8_t
        case MONO_TYPE_U2:
            Log::LogWarning("UInt16 serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
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
                Log::LogWarning("Value types serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
                //MonoClass* monoClass = mono_class_from_mono_type(monoType);

                return nullptr;
            }
        case MONO_TYPE_CLASS:
        {
            Log::LogWarning("Class types serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            //MonoClass* monoClass = mono_class_from_mono_type(monoType);

            return nullptr;
        }
        case MONO_TYPE_STRING:
            return new ScriptStringAttributeAccessor(monoClassField, Types::String);
        case MONO_TYPE_ARRAY:
        case MONO_TYPE_SZARRAY:
            Log::LogWarning("Array types serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr;
        case MONO_TYPE_GENERICINST:
            Log::LogWarning("Generic types serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr;
        case MONO_TYPE_OBJECT:
            Log::LogWarning("Objects serialization is not supported yet: {0}", mono_field_get_name(monoClassField));
            return nullptr;
        default:
            Log::LogWarning("Unknown field type {1}: {0}", mono_field_get_name(monoClassField), monoTypeEnum);
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

void ScriptingCore::CallCallbackMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method)
{
    void* params[2];
    params[0] = &ownerEntityID;
    params[1] = &callbackType;
    CallMethod(method, nullptr, params);
}

void ScriptingCore::FindAndCallEntryPoint()
{
    if (Domain->CustomAssemblyImage == nullptr)
    {
        Log::LogWarning("Custom assembly is null");
        return;
    }

    Log::LogDebug("FindAndCallEntryPoint");

    MonoClass* klass = mono_class_from_name(Domain->CustomAssemblyImage, "SteelCustom", "GameManager");
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

    CallMethod(entryPointMethod, nullptr, nullptr);
}

const char* ScriptingCore::ToString(MonoString* monoString)
{
    if (monoString == nullptr)
        return "";
    const char* result = mono_string_to_utf8(monoString);
    return result ? result : "";
}

MonoString* ScriptingCore::FromString(const char* normalString)
{
    return mono_string_new(Domain->Domain, normalString);
}

MonoArray* ScriptingCore::ToMonoUInt32Array(const std::vector<uint32_t>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_uint32_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, uint32_t, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoUInt64Array(const std::vector<uint64_t>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_uint64_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, uint64_t, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoIntPtrArray(const std::vector<intptr_t>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_intptr_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, intptr_t, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoStringArray(const std::vector<std::string>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_string_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, MonoString*, i, FromString(inArray[i].c_str()));
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoFloatArray(const std::vector<float>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_single_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, float, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoIntArray(const std::vector<int>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_int32_class(), inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, int, i, inArray[i]);
    }

    return outArray;
}

MonoArray* ScriptingCore::ToMonoObjectArray(const std::vector<MonoObject*>& inArray)
{
    MonoArray* outArray = mono_array_new(Domain->Domain, mono_get_object_class(), inArray.size());

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

MonoMethod* ScriptingCore::GetInvokeCallbacksMethod()
{
    return Domain->InvokeCallbacksMethod;
}

MonoMethod* ScriptingCore::GetDeregisterCallbacksMethod()
{
    return Domain->DeregisterCallbacksMethod;
}

MonoMethod* ScriptingCore::GetCoroutinesUpdateMethod()
{
    return Domain->CoroutinesUpdateMethod;
}
