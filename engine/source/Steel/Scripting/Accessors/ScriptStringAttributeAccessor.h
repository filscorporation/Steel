#pragma once

#include "ScriptStringAttributeAccessor.h"
#include "Steel/Scripting/ScriptingCore.h"

// Script attribute accessor for mono string
class ScriptStringAttributeAccessor : public ScriptAttributeAccessorBase
{
public:
    ScriptStringAttributeAccessor(MonoClassField* monoClassField, Types::Type type)
            : _monoClassField(monoClassField), _type(type)
    {

    }

    Types::Type GetType() const override
    {
        return _type;
    }

    void Serialize(ScriptObjectHandler* scriptHandler, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = ScriptingCore::ToString(Get(scriptHandler));
    }

    void Deserialize(ScriptObjectHandler* scriptHandler, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(scriptHandler, ScriptingCore::FromString(node[name].as<std::string>().c_str()));
    }

private:
    MonoString* Get(ScriptObjectHandler* scriptHandler) const
    {
        return ScriptingCore::GetScriptPtrFieldValue<MonoString*>(scriptHandler, _monoClassField);
    }

    void Set(ScriptObjectHandler* scriptHandler, MonoString* value) const
    {
        ScriptingCore::SetScriptPtrFieldValue<MonoString*>(scriptHandler, _monoClassField, value);
    }

    MonoClassField* _monoClassField;
    Types::Type _type;
};
