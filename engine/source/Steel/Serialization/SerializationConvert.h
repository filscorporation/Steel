#pragma once

#include "Steel/UI/TransitionInfo.h"

#include <glm/vec3.hpp>
#include <yaml-cpp/yaml.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace YAML
{
    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& value)
        {
            Node node;
            node.SetStyle(EmitterStyle::Flow);
            node.push_back(value.x);
            node.push_back(value.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& value)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            value.x = node[0].as<float>();
            value.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& value)
        {
            Node node;
            node.SetStyle(EmitterStyle::Flow);
            node.push_back(value.x);
            node.push_back(value.y);
            node.push_back(value.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& value)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            value.x = node[0].as<float>();
            value.y = node[1].as<float>();
            value.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& value)
        {
            Node node;
            node.SetStyle(EmitterStyle::Flow);
            node.push_back(value.r);
            node.push_back(value.g);
            node.push_back(value.b);
            node.push_back(value.a);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& value)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            value.r = node[0].as<float>();
            value.g = node[1].as<float>();
            value.b = node[2].as<float>();
            value.a = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::quat>
    {
        static Node encode(const glm::quat& value)
        {
            Node node;
            node.SetStyle(EmitterStyle::Flow);
            node.push_back(value.w);
            node.push_back(value.x);
            node.push_back(value.y);
            node.push_back(value.z);
            return node;
        }

        static bool decode(const Node& node, glm::quat& value)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            value.w = node[0].as<float>();
            value.x = node[1].as<float>();
            value.y = node[2].as<float>();
            value.z = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<TransitionsInfo>
    {
        static Node encode(const TransitionsInfo& value)
        {
            Node node;
            node.SetStyle(EmitterStyle::Flow);
            node.push_back((int)value.TransitionType);
            node.push_back(value.TransitionDuration);
            node.push_back(value.Normal.Value);
            node.push_back(value.Hovered.Value);
            node.push_back(value.Selected.Value);
            node.push_back(value.Clicked.Value);
            node.push_back(value.Disabled.Value);
            return node;
        }

        static bool decode(const Node& node, TransitionsInfo& value)
        {
            if (!node.IsSequence() || node.size() != 7)
                return false;

            value.TransitionType = (TransitionTypes::TransitionType)node[0].as<int>();
            value.TransitionDuration = node[1].as<float>();
            value.Normal.Value = node[2].as<uint64_t>();
            value.Hovered.Value = node[3].as<uint64_t>();
            value.Selected.Value = node[4].as<uint64_t>();
            value.Clicked.Value = node[5].as<uint64_t>();
            value.Disabled.Value = node[6].as<uint64_t>();
            return true;
        }
    };
}
