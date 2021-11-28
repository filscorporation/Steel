#pragma once

#include "AttributeAccessor.h"
#include "Steel/Common/FlagType.h"

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
    AttributeInfo(const std::string& name, AttributeAccessorBase* accessor, AttributeFlags::AttributeFlag flags)
        : _name(name), _accessor(accessor), _flags(flags)
    {

    }

    std::string Name() const { return _name; }
    AttributeFlags::AttributeFlag Flags() const { return _flags; }

    std::string ToString(Serializable* object) const
    {
        return _accessor->ToString(object);
    }

    void FromString(Serializable* object, const std::string& line)
    {
        _accessor->FromString(object, line);
    }

    void Serialize(Serializable* object, YAML::Node& node)
    {
        _accessor->Serialize(object, _name, node);
    }

    void Deserialize(Serializable* object, YAML::Node& node)
    {
        _accessor->Deserialize(object, _name, node);
    }

private:
    std::string _name;
    AttributeAccessorBase* _accessor = nullptr;
    AttributeFlags::AttributeFlag _flags;

    void Clear()
    {
        delete _accessor;
    }

    friend class SerializationManager;
};
