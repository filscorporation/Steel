#pragma once

#include <functional>

#include "../EntityComponentSystem/Entity.h"
#include "UIComponent.h"
#include "TransitionInfo.h"
#include "UIEvent.h"

typedef std::function<bool(EntityID)> UpdateInteractable;

class UIInteractable : public UIComponent
{
public:
    explicit UIInteractable(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void SetTransitionsInfo(TransitionsInfo info);
    TransitionsInfo GetTransitionsInfo() const;

    void SetTargetImage(EntityID targetID);
    EntityID GetTargetImage() const;
    void SetInteractable(bool interactable);
    bool GetInteractable() const;

protected:
    void Init(UpdateInteractable callback);
    void PlayTransition(TransitionStates::TransitionState state);
    void StopTransition();
    bool UpdateTransition();
    void RestoreTransition();

    bool IsInteractable = true;
    bool IsHovered = false;
    bool IsSelected = false;
    TransitionsInfo CurrentTransitionsInfo {};

private:
    EntityID _targetImage = NULL_ENTITY;

    UpdateInteractable updateCallback = nullptr;
    bool isInTransition = false;
    bool startingDataInitialized = false;
    TransitionStates::TransitionState currentState = TransitionStates::Normal;
    TransitionData startingTransitionData {};
    TransitionData targetTransitionData {};
    float transitionProgress = 0.0f;

    friend class UISystem;
};
