#pragma once

#include "Steel/Scripting/ScriptingCommon.h"
#include "Steel/Serialization/SerializationContext.h"
#include "Steel/EntityComponentSystem/Types.h"

#include <yaml-cpp/yaml.h>

// Base class to get/set data to script attributes (like AttributeAccessorBase, but for script objects)
class ScriptAttributeAccessorBase
{
public:
    virtual ~ScriptAttributeAccessorBase() = default;

    virtual Types::Type GetType() const = 0;
    virtual void Serialize(ScriptObjectHandler* scriptHandler, const std::string& name, YAML::Node& node, SerializationContext& context) = 0;
    virtual void Deserialize(ScriptObjectHandler* scriptHandler, const std::string& name, const YAML::Node& node, SerializationContext& context) = 0;
};
