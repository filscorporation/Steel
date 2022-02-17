#pragma once

#include "SerializationContext.h"
#include "Steel/EntityComponentSystem/Entity.h"

// Attribute accessor for enum fields
template <typename T, typename U>
class EnumAttributeAccessor : public AttributeAccessorBase
{
public:
    typedef U (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(U);

    EnumAttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    Types::Type GetType() const override
    {
        return Types::Enum;
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = static_cast<int>(Get(object));
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(object, static_cast<U>(node[name].as<int>()));
    }

private:
    U Get(Serializable* object) const
    {
        T* castedObject = static_cast<T*>(object);
        return (castedObject->*_getFunction)();
    }

    void Set(Serializable* object, const U& value) const
    {
        T* castedObject = static_cast<T*>(object);
        (castedObject->*_setFunction)(value);
    }

    GetFunction _getFunction;
    SetFunction _setFunction;
};
