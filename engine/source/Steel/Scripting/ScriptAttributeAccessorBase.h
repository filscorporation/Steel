#pragma once

#include "ScriptingCommon.h"

// Base class to get/set data to script attributes (like AttributeAccessorBase, but for script objects)
class ScriptAttributeAccessorBase
{
public:
    virtual ~ScriptAttributeAccessorBase() = default;

    virtual Types::Type GetType() const = 0;
    virtual void Serialize(ScriptPointer* pointer, const std::string& name, YAML::Node& node, SerializationContext& context) = 0;
    virtual void Deserialize(ScriptPointer* pointer, const std::string& name, const YAML::Node& node, SerializationContext& context) = 0;
    virtual void Copy(ScriptPointer* pointerFrom, ScriptPointer* pointerTo, SerializationContext& contextFrom, SerializationContext& contextTo) = 0;
};
