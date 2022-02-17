#pragma once

#include "ScriptingCommon.h"
#include "SimpleAPITypes.h"
#include "APIStructs.h"
#include "Steel/EntityComponentSystem/TypeInfo.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>
#include <unordered_map>

struct ScriptingDomain
{
    MonoDomain* Domain = nullptr;
    MonoImage* CoreAssemblyImage = nullptr;
    MonoImage* CustomAssemblyImage = nullptr;

    std::unordered_map<MonoClass*, const TypeInfo*> CachedAPITypes;
    std::unordered_map<SimpleAPITypes::SimpleAPIType, MonoClass*> CachedSimpleAPITypes;
    std::unordered_map<APIStructs::APIStruct, std::pair<MonoClass*, MonoMethod*>> CachedAPIStructs;
    std::unordered_map<MonoClass*, ScriptTypeInfo*> ScriptsInfo;
    std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> EventMethodsDescriptions;

    MonoClass* BaseScriptClass = nullptr;
    MonoClass* SerializableAttributeClass = nullptr;
    MonoClass* ComponentClass = nullptr;
    MonoClass* EntityClass = nullptr;
    MonoMethod* EntityConstructorMethod = nullptr;
    MonoProperty* ComponentEntityProperty = nullptr;

    MonoMethod* InvokeCallbacksMethod = nullptr;
    MonoMethod* DeregisterCallbacksMethod = nullptr;
    MonoMethod* CoroutinesUpdateMethod = nullptr;
};
