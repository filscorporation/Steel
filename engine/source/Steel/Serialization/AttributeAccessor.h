#pragma once

#include "SerializationContext.h"
#include "Steel/Common/StringUtils.h"
#include "Steel/EntityComponentSystem/Entity.h"

// Template attribute accessor using getter setter functions
template <typename T, typename U, typename Typedef>
class AttributeAccessor : public AttributeAccessorBase
{
public:
    typedef typename Typedef::ReturnType (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(typename Typedef::ParameterType);

    AttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    void FromString(Serializable* object, const std::string& line, SerializationContext& context) override
    {
        Set(object, StringUtils::FromString<U>(line));
    }

    std::string ToString(Serializable* object, SerializationContext& context) const override
    {
        return StringUtils::ToString(Get(object));
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = Get(object);
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(object, node[name].as<U>());
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
