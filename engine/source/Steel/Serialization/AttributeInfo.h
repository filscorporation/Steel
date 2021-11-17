#pragma once

#include "AttributeAccessor.h"

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

    inline AttributeFlag operator|(AttributeFlag a, AttributeFlag b)
    {
        return static_cast<AttributeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline AttributeFlag operator&(AttributeFlag a, AttributeFlag b)
    {
        return static_cast<AttributeFlag>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
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

    // TODO: void Serialize(Serializable* object, Node* node)
    // TODO: void Deserialize(Serializable* object, Node* node)

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
