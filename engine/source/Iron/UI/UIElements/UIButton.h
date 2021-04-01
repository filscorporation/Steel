#pragma once

#include "../UIComponent.h"
#include "../UIRenderer.h"
#include "../UIEvent.h"
#include "UIImage.h"
#include "../../Rendering/Sprite.h"

namespace ButtonTransitionTypes
{
    enum ButtonTransitionType
    {
        ColorShift,
        SpriteChange,
        SpriteTileChange,
        Animation,
    };
}

struct ButtonTransitionData
{
    uint32_t Value;

    glm::vec4 ToColor() const
    {
        return glm::vec4((Value >> 24) / 255.0f, (Value << 8 >> 24) / 255.0f,
                         (Value << 16 >> 24) / 255.0f, (Value << 24 >> 24) / 255.0f);
    }

    void FromColor(glm::vec4 color)
    {
        Value = ((uint32_t)(color.r * 255) << 24) + ((uint32_t)(color.g * 255) << 16)
              + ((uint32_t)(color.b * 255) << 8) + ((uint32_t)(color.a * 255));
    }
};

struct ButtonTransitionsInfo
{
    ButtonTransitionTypes::ButtonTransitionType TransitionType;
    float TransitionDuration;
    ButtonTransitionData Normal;
    ButtonTransitionData Hovered;
    ButtonTransitionData Clicked;
    ButtonTransitionData Disabled;
};

typedef void(*ButtonCallback)();

class UIButton : public UIImage
{
public:
    explicit UIButton(EntityID ownerEntityID) : UIImage(ownerEntityID) { };

    bool Update();

    void SetTransitionsInfo(ButtonTransitionsInfo info);
    ButtonTransitionsInfo GetTransitionsInfo() const;

    void SetInteractable(bool interactable);
    bool GetInteractable() const;

    ButtonCallback Callback = nullptr;

private:
    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void PlayTransition(ButtonTransitionData data);

    bool _interactable = true;
    ButtonTransitionsInfo _transitionsInfo {};

    bool isHovered = false;
    bool isInTransition = false;
    ButtonTransitionData targetTransitionData;
    float transitionProgress = 0.0f;

    friend class UISystem;
};
