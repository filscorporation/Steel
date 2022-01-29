#pragma once

#include "SerializationContext.h"

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

    virtual std::string ToString(Serializable* object, SerializationContext& context) const = 0;
    virtual void FromString(Serializable* object, const std::string& line, SerializationContext& context) = 0;

    virtual void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) = 0;
    virtual void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) = 0;
    virtual void Copy(Serializable* objectFrom, Serializable* objectTo, SerializationContext& contextFrom, SerializationContext& contextTo) = 0;
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

template <> struct AttributeTypedef<long>
{
    typedef long ReturnType;
    typedef long ParameterType;
};

template <> struct AttributeTypedef<uint32_t>
{
    typedef uint32_t ReturnType;
    typedef uint32_t ParameterType;
};

template <> struct AttributeTypedef<uint64_t>
{
    typedef uint64_t ReturnType;
    typedef uint64_t ParameterType;
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
