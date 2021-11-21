#pragma once

#include "../Common/StringUtils.h"
#include "../EntityComponentSystem/Entity.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <yaml-cpp/yaml.h>

class Serializable;

// Base attribute accessor
class AttributeAccessorBase
{
public:
    virtual ~AttributeAccessorBase() = default;

    virtual std::string ToString(Serializable* object) const = 0;
    virtual void FromString(Serializable* object, const std::string& line) = 0;

    virtual void Serialize(Serializable* object, const std::string& name, YAML::Node& node) = 0;
    virtual void Deserialize(Serializable* object, const std::string& name, YAML::Node& node) = 0;
};

template <typename T> struct AttributeTypedef
{
    typedef const T& ReturnType;
    typedef const T& ParameterType;
};

template <> struct AttributeTypedef<int>
{
    typedef int ReturnType;
    typedef int ParameterType;
};

template <> struct AttributeTypedef<uint32_t>
{
    typedef uint32_t ReturnType;
    typedef uint32_t ParameterType;
};

template <> struct AttributeTypedef<bool>
{
    typedef bool ReturnType;
    typedef bool ParameterType;
};

template <> struct AttributeTypedef<float>
{
    typedef float ReturnType;
    typedef float ParameterType;
};

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

    void FromString(Serializable* object, const std::string& line) override
    {
        Set(object, StringUtils::FromString<U>(line));
    }

    std::string ToString(Serializable* object) const override
    {
        return StringUtils::ToString(Get(object));
    }

    void Serialize(Serializable* object, const std::string& name, YAML::Node& node) override
    {
        node[name] = Get(object);
    }

    void Deserialize(Serializable* object, const std::string& name, YAML::Node& node) override
    {
        Set(object, node[name].as<U>());
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
