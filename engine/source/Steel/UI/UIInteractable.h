#pragma once

#include <functional>

#include "UIComponent.h"
#include "TransitionInfo.h"
#include "UIEvent.h"
#include "Steel/EntityComponentSystem/Entity.h"

typedef std::function<bool(EntityID)> UpdateInteractable;

class UIInteractable : public UIComponent
{
public:
    explicit UIInteractable(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void SetTransitionsInfo(const TransitionsInfo& info);
    const TransitionsInfo& GetTransitionsInfo() const;

    void SetTargetImage(EntityID targetID);
    EntityID GetTargetImage() const;
    void SetInteractable(bool interactable);
    bool GetInteractable() const;

    void StopTransition();
    void RestoreTransition();

protected:
    void SetDefaultTransitionInfo();
    void InitTransitionInfo(UpdateInteractable callback);
    void PlayTransition(TransitionStates::TransitionState state);
    bool UpdateTransition();

    bool IsInteractable = true;
    bool IsHovered = false;
    bool IsSelected = false;

private:
    EntityID _targetImage = NULL_ENTITY;

    TransitionsInfo currentTransitionsInfo {};
    UpdateInteractable updateCallback = nullptr;
    bool isInTransition = false;
    bool startingDataInitialized = false;
    TransitionStates::TransitionState currentState = TransitionStates::Normal;
    TransitionData startingTransitionData {};
    TransitionData targetTransitionData {};
    float transitionProgress = 0.0f;
};
