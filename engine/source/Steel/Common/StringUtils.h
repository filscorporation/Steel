#pragma once

#include "Steel/Serialization/Serializable.h"
#include "Steel/UI/TransitionInfo.h"

#include <glm/glm.hpp>
#include <sstream>

namespace StringUtils
{
    template<class T>
    static std::string ToString(const T& value)
    {
        return std::to_string(value);
    }

    template<>
    std::string ToString(const std::string& value)
    {
        return value;
    }

    template<>
    std::string ToString(const glm::vec2& value)
    {
        std::stringstream ss;
        ss << "(" << value.x << ", " << value.y << ")";
        return ss.str();
    }

    template<>
    std::string ToString(const glm::vec3& value)
    {
        std::stringstream ss;
        ss << "(" << value.x << ", " << value.y << ", " << value.z << ")";
        return ss.str();
    }

    template<>
    std::string ToString(const glm::vec4& value)
    {
        std::stringstream ss;
        ss << "(" << value.r << ", " << value.g << ", " << value.b << ", " << value.a << ")";
        return ss.str();
    }

    template<>
    std::string ToString(const TransitionsInfo& value)
    {
        std::stringstream ss;
        ss << "(" << (int)value.TransitionType << ", " << value.TransitionDuration << ", "
            << value.Normal.Value << ", "
            << value.Hovered.Value << ", "
            << value.Selected.Value << ", "
            << value.Clicked.Value << ", "
            << value.Disabled.Value << ")";
        return ss.str();
    }

    template<class T>
    static T FromString(const std::string& line)
    {
        return line;
    }

    template<>
    int FromString(const std::string& line)
    {
        return std::stoi(line);
    }

    template<>
    long FromString(const std::string& line)
    {
        return std::stol(line);
    }

    template<>
    bool FromString(const std::string& line)
    {
        return (bool)std::stoi(line);
    }

    template<>
    float FromString(const std::string& line)
    {
        return std::stof(line);
    }

    template<>
    uint32_t FromString(const std::string& line)
    {
        return std::stoul(line);
    }

    template<>
    uint64_t FromString(const std::string& line)
    {
        return std::stoull(line);
    }

    template<>
    glm::vec2 FromString(const std::string& line)
    {
        std::stringstream ss(line);
        glm::vec2 result;
        char c;
        ss >> c >> result.x >> c >> result.y >> c;
        return result;
    }

    template<>
    glm::vec3 FromString(const std::string& line)
    {
        std::stringstream ss(line);
        glm::vec3 result;
        char c;
        ss >> c >> result.x >> c >> result.y >> c >> result.z;
        return result;
    }

    template<>
    glm::vec4 FromString(const std::string& line)
    {
        std::stringstream ss(line);
        glm::vec4 result;
        char c;
        ss >> c >> result.r >> c >> result.g >> c >> result.b >> c >> result.a;
        return result;
    }

    template<>
    TransitionsInfo FromString(const std::string& line)
    {
        std::stringstream ss(line);
        TransitionsInfo result {};
        char c;
        int typeAsInt;
        ss >> c >> typeAsInt >> c >> result.TransitionDuration >> c
            >> result.Normal.Value >> c
            >> result.Hovered.Value >> c
            >> result.Selected.Value >> c
            >> result.Clicked.Value >> c
            >> result.Disabled.Value >> c;
        result.TransitionType = (TransitionTypes::TransitionType)typeAsInt;
        return result;
    }
};
