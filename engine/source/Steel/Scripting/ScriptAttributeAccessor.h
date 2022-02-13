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

    void Serialize(ScriptPointer* pointer, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = Get(pointer);
    }

    void Deserialize(ScriptPointer* pointer, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(pointer, node[name].as<U>());
    }

    void Copy(ScriptPointer* pointerFrom, ScriptPointer* pointerTo, SerializationContext& contextFrom, SerializationContext& contextTo) override
    {
        Set(pointerTo, Get(pointerFrom));
    }

private:
    U Get(ScriptPointer* pointer) const
    {
        return ScriptingCore::GetScriptingFieldValue<U>(pointer, _monoClassField);
    }

    void Set(ScriptPointer* pointer, const U& value) const
    {
        ScriptingCore::SetScriptingFieldValue<U>(pointer, _monoClassField, value);
    }

    MonoClassField* _monoClassField;
    Types::Type _type;
};
