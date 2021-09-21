#pragma once

#include "../Math/Math.h"

namespace TransitionTypes
{
    enum TransitionType
    {
        ColorShift,
        SpriteChange,
        SpriteTileChange,
        Animation,
    };
}

struct TransitionData
{
    uint32_t Value;

    glm::vec4 ToColor() const
    {
        return glm::vec4(Math::Clamp01((Value >> 24) / 255.0f),
                         Math::Clamp01((Value << 8 >> 24) / 255.0f),
                         Math::Clamp01((Value << 16 >> 24) / 255.0f),
                         Math::Clamp01((Value << 24 >> 24) / 255.0f));
    }

    void FromColor(glm::vec4 color)
    {
        Value = ((uint32_t)(Math::Clamp01(color.r) * 255) << 24) + ((uint32_t)(Math::Clamp01(color.g) * 255) << 16)
                + ((uint32_t)(Math::Clamp01(color.b) * 255) << 8) + ((uint32_t)(Math::Clamp01(color.a) * 255));
    }
};

struct TransitionsInfo
{
    TransitionTypes::TransitionType TransitionType;
    float TransitionDuration;
    TransitionData Normal;
    TransitionData Hovered;
    TransitionData Selected;
    TransitionData Clicked;
    TransitionData Disabled;
};
