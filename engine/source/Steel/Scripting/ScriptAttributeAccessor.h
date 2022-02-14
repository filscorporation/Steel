#pragma once

#include "ScriptAttributeAccessorBase.h"
#include "ScriptingCore.h"

// Template script attribute accessor for simple types
template <typename U>
class ScriptAttributeAccessor : public ScriptAttributeAccessorBase
{
public:
    ScriptAttributeAccessor(MonoClassField* monoClassField, Types::Type type)
            : _monoClassField(monoClassField), _type(type)
    {

    }

    Types::Type GetType() const override
    {
        return _type;
    }

    void Serialize(ScriptObjectHandler* scriptHandler, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = Get(scriptHandler);
    }

    void Deserialize(ScriptObjectHandler* scriptHandler, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(scriptHandler, node[name].as<U>());
    }

    void Copy(ScriptObjectHandler* scriptHandlerFrom, ScriptObjectHandler* scriptHandlerTo, SerializationContext& contextFrom, SerializationContext& contextTo) override
    {
        Set(scriptHandlerTo, Get(scriptHandlerFrom));
    }

private:
    U Get(ScriptObjectHandler* scriptHandler) const
    {
        return ScriptingCore::GetScriptingFieldValue<U>(scriptHandler, _monoClassField);
    }

    void Set(ScriptObjectHandler* scriptHandler, const U& value) const
    {
        ScriptingCore::SetScriptingFieldValue<U>(scriptHandler, _monoClassField, value);
    }

    MonoClassField* _monoClassField;
    Types::Type _type;
};
