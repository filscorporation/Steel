#pragma once

#include "SerializationContext.h"
#include "AttributeAccessorBase.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Scripting/ScriptAttributeAccessorBase.h"
#include "Steel/Scripting/ScriptingCommon.h"
//#include "Steel/Scripting/ScriptingCore.h"

template <typename T>
class ScriptsListAttributeAccessor : public AttributeAccessorBase
{
public:
    typedef const std::vector<ScriptData>& (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(const std::vector<ScriptData>&);

    ScriptsListAttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    Types::Type GetType() const override
    {
        return Types::Script;
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        auto list = Get(object);
        for (auto& scriptData : list)
        {
            // TODO: hide work with type info in ScriptComponent
            YAML::Node scriptNode;
            scriptNode["type"] = scriptData.TypeInfo->TypeNamespace + "." + scriptData.TypeInfo->TypeName;
            for (auto field : scriptData.TypeInfo->Attributes)
            {
                field.Accessor->Serialize(scriptData.ScriptHandler, field.FieldName, scriptNode, context);
            }

            node.push_back(scriptNode);
        }
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        std::vector<ScriptData> list;
        list.resize(node.size());
        for (auto& scriptNode : node)
        {
            // TODO: hide work with type info in ScriptComponent
            ScriptData scriptData {};
            auto typeName = scriptNode[0].as<std::string>();
            std::string classNamespace = typeName.substr(typeName.find_last_of('.') + 1);
            std::string className = typeName.substr(typeName.find_last_of('.') + 1);

            //MonoClass* monoClass = ScriptingCore::GetMonoClassByFullName(classNamespace, className);
            //if (monoClass == nullptr)
            //{
            //    Log::LogError("Couldn't find mono class {0}", typeName);
            //    continue;
            //}

            ScriptTypeInfo* typeInfo = nullptr;//ScriptingCore::ScriptParseRecursive(monoClass);
            ScriptObjectHandler* handler = nullptr;//ScriptingCore::CreateManagedInstance(monoClass, nullptr, 0);
            if (handler == nullptr)
            {
                Log::LogError("Couldn't create instance of class {0}", typeName);
                continue;
            }

            // Entity owner for created instance is set later in deserialization process - in ScriptComponent::OnCreate()

            scriptData.TypeInfo = typeInfo;
            scriptData.ScriptHandler = handler;

            for (auto field : scriptData.TypeInfo->Attributes)
            {
                field.Accessor->Deserialize(scriptData.ScriptHandler, field.FieldName, scriptNode, context);
            }
        }
        Set(object, list);
    }

private:
    const std::vector<ScriptData>& Get(Serializable* object) const
    {
        T* castedObject = static_cast<T*>(object);
        return (castedObject->*_getFunction)();
    }

    void Set(Serializable* object, const std::vector<ScriptData>& value) const
    {
        T* castedObject = static_cast<T*>(object);
        (castedObject->*_setFunction)(value);
    }

    GetFunction _getFunction;
    SetFunction _setFunction;
};
