#pragma once

#include "SerializationContext.h"
#include "Steel/EntityComponentSystem/Types.h"

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

    virtual Types::Type GetType() const = 0;
    virtual void Serialize(Serializable* object, const std::string& name, YAML::Node& node, SerializationContext& context) = 0;
    virtual void Deserialize(Serializable* object, const std::string& name, const YAML::Node& node, SerializationContext& context) = 0;
    virtual void Copy(Serializable* objectFrom, Serializable* objectTo, SerializationContext& contextFrom, SerializationContext& contextTo) = 0;
};

template <typename T> struct AttributeTypedef
{
    typedef const T& ReturnType;
    typedef const T& ParameterType;

    static const Types::Type AttributeType = Types::Undefined;
};

template <> struct AttributeTypedef<int>
{
    typedef int ReturnType;
    typedef int ParameterType;

    static const Types::Type AttributeType = Types::Int;
};

template <> struct AttributeTypedef<long>
{
    typedef long ReturnType;
    typedef long ParameterType;

    static const Types::Type AttributeType = Types::Long;
};

template <> struct AttributeTypedef<uint32_t>
{
    typedef uint32_t ReturnType;
    typedef uint32_t ParameterType;

    static const Types::Type AttributeType = Types::UInt32;
};

template <> struct AttributeTypedef<uint64_t>
{
    typedef uint64_t ReturnType;
    typedef uint64_t ParameterType;

    static const Types::Type AttributeType = Types::UInt64;
};

template <> struct AttributeTypedef<bool>
{
    typedef bool ReturnType;
    typedef bool ParameterType;

    static const Types::Type AttributeType = Types::Bool;
};

template <> struct AttributeTypedef<float>
{
    typedef float ReturnType;
    typedef float ParameterType;

    static const Types::Type AttributeType = Types::Float;
};

template <> struct AttributeTypedef<std::string>
{
    typedef const std::string& ReturnType;
    typedef const std::string& ParameterType;

    static const Types::Type AttributeType = Types::String;
};

template <> struct AttributeTypedef<glm::vec2>
{
    typedef const glm::vec2& ReturnType;
    typedef const glm::vec2& ParameterType;

    static const Types::Type AttributeType = Types::Vector2;
};

template <> struct AttributeTypedef<glm::vec3>
{
    typedef const glm::vec3& ReturnType;
    typedef const glm::vec3& ParameterType;

    static const Types::Type AttributeType = Types::Vector3;
};

template <> struct AttributeTypedef<glm::vec4>
{
    typedef const glm::vec4& ReturnType;
    typedef const glm::vec4& ParameterType;

    static const Types::Type AttributeType = Types::Color;
};
