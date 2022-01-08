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

namespace TransitionStates
{
    enum TransitionState
    {
        Normal,
        Hovered,
        Selected,
        Clicked,
        Disabled,
    };
}

struct TransitionData
{
    uint64_t Value;

    glm::vec4 ToColor() const
    {
        return {Math::Clamp01((float)((uint32_t)Value >> 24) / 255.0f),
                Math::Clamp01((float)((uint32_t)Value << 8 >> 24) / 255.0f),
                Math::Clamp01((float)((uint32_t)Value << 16 >> 24) / 255.0f),
                Math::Clamp01((float)((uint32_t)Value << 24 >> 24) / 255.0f)};
    }

    void FromColor(glm::vec4 color)
    {
        Value = ((uint32_t)(Math::Clamp01(color.r) * 255) << 24) + ((uint32_t)(Math::Clamp01(color.g) * 255) << 16)
                + ((uint32_t)(Math::Clamp01(color.b) * 255) << 8) + ((uint32_t)(Math::Clamp01(color.a) * 255));
    }
};

struct TransitionsInfo
{
    TransitionData Get(TransitionStates::TransitionState state) const
    {
        switch (state)
        {
            case TransitionStates::Normal:
                return Normal;
            case TransitionStates::Hovered:
                return Hovered;
            case TransitionStates::Selected:
                return Selected;
            case TransitionStates::Clicked:
                return Clicked;
            case TransitionStates::Disabled:
                return Disabled;
        }
        return Normal;
    }

    TransitionTypes::TransitionType TransitionType;
    float TransitionDuration;
    TransitionData Normal;
    TransitionData Hovered;
    TransitionData Selected;
    TransitionData Clicked;
    TransitionData Disabled;
};
