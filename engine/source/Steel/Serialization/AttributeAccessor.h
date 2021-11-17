#pragma once

#include "../Common/StringUtils.h"
#include "../EntityComponentSystem/Entity.h"

#include <string>
#include <glm/glm.hpp>

class Serializable;

// Base attribute accessor
class AttributeAccessorBase
{
public:
    virtual ~AttributeAccessorBase() = default;

    virtual std::string ToString(Serializable* object) const = 0;
    virtual void FromString(Serializable* object, const std::string& line) = 0;

    // TODO: void Serialize(Serializable* object, Node* node)
    // TODO: void Deserialize(Serializable* object, Node* node)
};

// Template attribute accessor
template <typename T, typename U> class AttributeAccessor : public AttributeAccessorBase
{
public:
    typedef U (T::*GetFunction)() const;
    typedef void (T::*SetFunction)(U);

    AttributeAccessor(GetFunction getFunction, SetFunction setFunction)
            : _getFunction(getFunction), _setFunction(setFunction)
    {

    }

    void FromString(Serializable* object, const std::string& line) override
    {
        Set(object, StringUtils::FromString<U>(line));
    }

    std::string ToString(Serializable* object) const override
    {
        return StringUtils::ToString(Get(object));
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
