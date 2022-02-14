#pragma once

#include "SerializationContext.h"
#include "AttributeAccessorBase.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Scripting/ScriptAttributeAccessorBase.h"
#include "Steel/Scripting/ScriptingCommon.h"

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
            // TODO: finish
            ScriptTypeInfo* typeInfo = nullptr; // ScriptingCore::GetTypeInfoByName(typeName)
            ScriptObjectHandler* handler = nullptr; // ScriptingCore::CreateInstanceOfType(typeName)

            scriptData.TypeInfo = typeInfo;
            scriptData.ScriptHandler = handler;

            for (auto field : scriptData.TypeInfo->Attributes)
            {
                field.Accessor->Deserialize(scriptData.ScriptHandler, field.FieldName, scriptNode, context);
            }
        }
        Set(object, list);
    }

    void Copy(Serializable* objectFrom, Serializable* objectTo, SerializationContext& contextFrom, SerializationContext& contextTo) override
    {
        // TODO
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
