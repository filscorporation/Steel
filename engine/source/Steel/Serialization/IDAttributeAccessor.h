#pragma once

#include "SerializationContext.h"
#include "Steel/EntityComponentSystem/Entity.h"

// Attribute accessor for EntityID <-> UUID linked fields
template <typename T>
class IDAttributeAccessor : public AttributeAccessorBase
{
public:
    typedef EntityID (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(EntityID);

    IDAttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    Types::Type GetType() const override
    {
        return Types::EntityID;
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        node[name] = context.GetUUID(Get(object));
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        Set(object, context.GetEntityID(node[name].as<UUID>()));
    }

    void Copy(Serializable *objectFrom, Serializable *objectTo, SerializationContext &contextFrom, SerializationContext &contextTo) override
    {
        Set(objectTo, contextTo.GetEntityID(contextFrom.GetUUID(Get(objectFrom))));
    }

private:
    EntityID Get(Serializable* object) const
    {
        T* castedObject = static_cast<T*>(object);
        return (castedObject->*_getFunction)();
    }

    void Set(Serializable* object, const EntityID& value) const
    {
        T* castedObject = static_cast<T*>(object);
        (castedObject->*_setFunction)(value);
    }

    GetFunction _getFunction;
    SetFunction _setFunction;
};
