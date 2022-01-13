#include "UIInteractable.h"

#include "UIElements/UIImage.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Time.h"

void UIInteractable::SetDefaultTransitionInfo()
{
    currentTransitionsInfo.TransitionType = TransitionTypes::ColorShift;
    currentTransitionsInfo.TransitionDuration = 0.1f;
    currentTransitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    currentTransitionsInfo.Hovered.FromColor(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
    currentTransitionsInfo.Selected.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    currentTransitionsInfo.Clicked.FromColor(glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));
    currentTransitionsInfo.Disabled.FromColor(glm::vec4(0.35f, 0.35f, 0.35f, 1.0f));
}

void UIInteractable::InitTransitionInfo(UpdateInteractable callback)
{
    updateCallback = callback;

    startingTransitionData = currentTransitionsInfo.Normal;
    targetTransitionData = currentTransitionsInfo.Normal;
}

bool UIInteractable::UpdateTransition()
{
    if (!isInTransition)
        return false;

    transitionProgress += Time::UnscaledDeltaTime();
    bool needInit = !startingDataInitialized;
    startingDataInitialized = true;

    if (_targetImage == NULL_ENTITY)
        return false;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (!entitiesRegistry->EntityExists(_targetImage) || !entitiesRegistry->HasComponent<UIImage>(_targetImage))
    {
        _targetImage = NULL_ENTITY;
        return false;
    }
    auto& image = entitiesRegistry->GetComponent<UIImage>(_targetImage);

    float t = Math::Clamp01(transitionProgress / currentTransitionsInfo.TransitionDuration);
    switch (currentTransitionsInfo.TransitionType)
    {
        case TransitionTypes::ColorShift:
            if (needInit)
                startingTransitionData.FromColor(image.GetColor());
            image.SetColor(Math::Lerp(startingTransitionData.ToColor(), targetTransitionData.ToColor(), t));
            break;
        case TransitionTypes::SpriteChange:
            image.SetImage(Application::Instance->GetResourcesManager()->GetSprite(targetTransitionData.Value));
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case TransitionTypes::SpriteTileChange:
            image.SetImageTileIndex(targetTransitionData.Value);
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case TransitionTypes::Animation:
            // TODO: not supported yet
            break;
    }

    if (transitionProgress > currentTransitionsInfo.TransitionDuration)
    {
        isInTransition = false;
        transitionProgress = 0.0f;
    }

    return isInTransition;
}

void UIInteractable::SetTransitionsInfo(TransitionsInfo info)
{
    currentTransitionsInfo = info;

    RestoreTransition();
}

TransitionsInfo UIInteractable::GetTransitionsInfo() const
{
    return currentTransitionsInfo;
}

void UIInteractable::SetTargetImage(EntityID targetID)
{
    _targetImage = targetID;
}

EntityID UIInteractable::GetTargetImage() const
{
    return _targetImage;
}

void UIInteractable::SetInteractable(bool interactable)
{
    if (IsInteractable == interactable)
        return;

    IsInteractable = interactable;
    IsHovered = false;

    PlayTransition(IsInteractable ? (IsHovered ? TransitionStates::Hovered : TransitionStates::Normal) : TransitionStates::Disabled);
}

bool UIInteractable::GetInteractable() const
{
    return IsInteractable;
}

void UIInteractable::PlayTransition(TransitionStates::TransitionState state)
{
    if (isInTransition)
    {
        transitionProgress = 1.0f - transitionProgress;
    }
    else
    {
        if (updateCallback != nullptr)
            Application::Instance->GetCurrentScene()->GetUILayer()->AddToUpdateQueue(Owner, updateCallback);
    }

    startingDataInitialized = false;
    currentState = state;
    targetTransitionData = currentTransitionsInfo.Get(currentState);
    isInTransition = true;
}

void UIInteractable::StopTransition()
{
    if (isInTransition)
    {
        isInTransition = false;
        Application::Instance->GetCurrentScene()->GetUILayer()->RemoveFromUpdateQueue(Owner);
    }
}

void UIInteractable::RestoreTransition()
{
    if (updateCallback != nullptr)
    {
        if (!isInTransition)
        {
            isInTransition = true;
            transitionProgress = 1.0f;
        }
        targetTransitionData = currentTransitionsInfo.Get(currentState);

        updateCallback(Owner);
    }
}
