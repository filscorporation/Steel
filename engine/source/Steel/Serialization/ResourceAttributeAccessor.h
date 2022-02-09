#pragma once

#include "SerializationContext.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Resources/ResourceID.h"

// Attribute accessor for resource links
template <typename T, typename U>
class ResourceAttributeAccessor : public AttributeAccessorBase
{
public:
    typedef U (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(U);

    ResourceAttributeAccessor(GetFunction getFunction, SetFunction setFunction, ResourceTypes::ResourceType resourceType)
            : _getFunction(getFunction), _setFunction(setFunction)
    {
        _resourceType = resourceType;
    }

    Types::Type GetType() const override
    {
        return Types::ResourceID;
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = context.GetResourceID(Get(object));
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(object, static_cast<U>(context.GetResource(_resourceType, node[name].as<ResourceID>())));
    }

    void Copy(Serializable *objectFrom, Serializable *objectTo, SerializationContext &contextFrom, SerializationContext &contextTo) override
    {
        Set(objectTo, static_cast<U>(contextTo.GetResource(_resourceType, contextFrom.GetResourceID(Get(objectFrom)))));
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
    ResourceTypes::ResourceType _resourceType = ResourceTypes::Undefined;
};
