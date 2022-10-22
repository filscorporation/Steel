#pragma once

#include "AttributeAccessorBase.h"
#include "Steel/Common/FlagType.h"
#include "Steel/Core/Log.h"
#include "SerializationContext.h"

#include <cstdint>
#include <string>

class Serializable;

namespace AttributeFlags
{
    enum AttributeFlag
    {
        Public   = 1 << 0,
        Readonly = 1 << 1,
    };

    FLAG_TYPE(AttributeFlag)
}

class AttributeInfo
{
public:
    AttributeInfo(const std::string& name, AttributeAccessorBase* accessor, AttributeFlags::AttributeFlag flags, const std::string& label)
        : _name(name), _accessor(accessor), _flags(flags), _label(label)
    {

    }

    std::string Name() const { return _name; }
    std::string Label() const { return _label; }
    AttributeFlags::AttributeFlag Flags() const { return _flags; }
    Types::Type GetType() const { return _accessor->GetType(); }

    void Serialize(Serializable* object, YAML::Node& node, SerializationContext& context) const
    {
        _accessor->Serialize(object, _name, node, context);
    }

    void Deserialize(Serializable* object, const YAML::Node& node, SerializationContext& context) const
    {
        if (!_accessor->RequireName() || node[_name])
            _accessor->Deserialize(object, _name, node, context);
    }

private:
    std::string _name;
    std::string _label;
    AttributeAccessorBase* _accessor = nullptr;
    AttributeFlags::AttributeFlag _flags;

    void Clear()
    {
        delete _accessor;
    }

    friend class SerializationManager;
};
