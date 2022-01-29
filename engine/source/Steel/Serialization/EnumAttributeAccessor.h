#pragma once

#include "SerializationContext.h"
#include "Steel/Common/StringUtils.h"
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

    void FromString(Serializable* object, const std::string& line, SerializationContext& context) override
    {
        Set(object, static_cast<U>(StringUtils::FromString<int>(line)));
    }

    std::string ToString(Serializable* object, SerializationContext& context) const override
    {
        return StringUtils::ToString(static_cast<int>(Get(object)));
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = static_cast<int>(Get(object));
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(object, static_cast<U>(node[name].as<int>()));
    }

    void Copy(Serializable* objectFrom, Serializable* objectTo, SerializationContext& contextFrom, SerializationContext& contextTo) override
    {
        Set(objectTo, Get(objectFrom));
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
