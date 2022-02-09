#pragma once

#include "SerializationContext.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "SerializationManager.h"

// Attribute accessor for lists fields
template <typename T, typename U>
class ListAttributeAccessor : public AttributeAccessorBase
{
public:
    typedef const std::vector<U>& (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(const std::vector<U>&);

    ListAttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    Types::Type GetType() const override
    {
        return Types::List; // TODO: list of what type?
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) override
    {
        auto list = Get(object);
        for (int i = 0; i < list.size(); ++i)
        {
            YAML::Node elementNode;
            auto element = static_cast<Serializable*>(&(list[i]));
            SerializationManager::Serialize(TYPE_ID(U), element, elementNode, context);
            node[name].push_back(elementNode);
        }
    }

    void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) override
    {
        std::vector<U> list;
        list.resize(node[name].size());
        for (int i = 0; i < node[name].size(); ++i)
        {
            auto element = static_cast<Serializable*>(&(list[i]));
            SerializationManager::Deserialize(TYPE_ID(U), element, node[name][i], context);
        }
        Set(object, list);
    }

    void Copy(Serializable* objectFrom, Serializable* objectTo, SerializationContext& contextFrom, SerializationContext& contextTo) override
    {
        auto listFrom = Get(objectFrom);
        std::vector<U> listTo;
        listTo.resize(listFrom.size());
        for (int i = 0; i < listTo.size(); ++i)
        {
            auto elementFrom = static_cast<Serializable*>(&(listFrom[i]));
            auto elementTo = static_cast<Serializable*>(&(listTo[i]));
            SerializationManager::Copy(TYPE_ID(U), elementFrom, elementTo, contextFrom, contextTo);
        }
        Set(objectTo, listTo);
    }

private:
    std::vector<U> Get(Serializable* object) const
    {
        T* castedObject = static_cast<T*>(object);
        return (castedObject->*_getFunction)();
    }

    void Set(Serializable* object, const std::vector<U>& value) const
    {
        T* castedObject = static_cast<T*>(object);
        (castedObject->*_setFunction)(value);
    }

    GetFunction _getFunction;
    SetFunction _setFunction;
};
