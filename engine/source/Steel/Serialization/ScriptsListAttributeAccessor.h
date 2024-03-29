#pragma once

#include "SerializationContext.h"
#include "AttributeAccessorBase.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Scripting/Accessors/ScriptAttributeAccessorBase.h"
#include "Steel/Scripting/ScriptingCommon.h"
#include "Steel/Scripting/ScriptSerializationHelper.h"

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

    bool RequireName() const override
    {
         return false;
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        auto list = Get(object);
        for (auto& scriptData : list)
        {
            YAML::Node scriptNode;
            scriptNode["type"] = scriptData.TypeInfo->TypeNamespace + "." + scriptData.TypeInfo->TypeName;
            YAML::Node dataNode;
            for (auto field : scriptData.TypeInfo->Attributes)
            {
                if (field.Accessor != nullptr)
                    field.Accessor->Serialize(scriptData.ScriptHandler, field.FieldName, dataNode, context);
            }
            scriptNode["data"] = dataNode;

            node.push_back(scriptNode);
        }
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        std::vector<ScriptData> list;
        list.reserve(node.size());
        for (auto& scriptNode : node)
        {
            ScriptData scriptData {};
            auto typeName = scriptNode["type"].as<std::string>();
            std::string classNamespace = typeName.substr(0, typeName.find_last_of('.'));
            std::string className = typeName.substr(typeName.find_last_of('.') + 1);

            if (!ScriptSerializationHelper::RestoreScriptInstance(classNamespace, className, scriptData))
                continue;

            // Entity owner for created instance is set later in deserialization process - in ScriptComponent::SetScriptsData()

            auto& dataNode = scriptNode["data"];
            for (auto field : scriptData.TypeInfo->Attributes)
            {
                if (dataNode[field.FieldName] && field.Accessor != nullptr)
                    field.Accessor->Deserialize(scriptData.ScriptHandler, field.FieldName, dataNode, context);
            }
            list.push_back(scriptData);
        }
        Set(object, list);
    }

private:
    const std::vector<ScriptData>& Get(Serializable* object) const
    {
        T* castedObject = dynamic_cast<T*>(object);
        return (castedObject->*_getFunction)();
    }

    void Set(Serializable* object, const std::vector<ScriptData>& value) const
    {
        T* castedObject = dynamic_cast<T*>(object);
        (castedObject->*_setFunction)(value);
    }

    GetFunction _getFunction;
    SetFunction _setFunction;
};
