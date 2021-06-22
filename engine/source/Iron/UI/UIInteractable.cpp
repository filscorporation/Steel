#include "UIInteractable.h"

#include "UIElements/UIImage.h"
#include "../Core/Log.h"
#include "../Core/Time.h"

void UIInteractable::Init(UpdateIntaractable callback)
{
    updateCallback = callback;
    CurrentTransitionsInfo.TransitionType = TransitionTypes::ColorShift;
    CurrentTransitionsInfo.TransitionDuration = 0.1f;
    CurrentTransitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    CurrentTransitionsInfo.Hovered.FromColor(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
    CurrentTransitionsInfo.Selected.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    CurrentTransitionsInfo.Clicked.FromColor(glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));
    CurrentTransitionsInfo.Disabled.FromColor(glm::vec4(0.35f, 0.35f, 0.35f, 1.0f));

    startingTransitionData = CurrentTransitionsInfo.Normal;
    targetTransitionData = CurrentTransitionsInfo.Normal;
}

bool UIInteractable::UpdateTransition()
{
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

    float t = Math::Clamp01(transitionProgress / CurrentTransitionsInfo.TransitionDuration);
    switch (CurrentTransitionsInfo.TransitionType)
    {
        case TransitionTypes::ColorShift:
            if (needInit)
                startingTransitionData.FromColor(image.GetColor());
            image.SetColor(Math::Lerp(startingTransitionData.ToColor(), targetTransitionData.ToColor(), t));
            break;
        case TransitionTypes::SpriteChange:
            image.SetImage(Application::Instance->GetResourcesManager()->GetImage(targetTransitionData.Value));
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

    if (transitionProgress > CurrentTransitionsInfo.TransitionDuration)
    {
        isInTransition = false;
        transitionProgress = 0.0f;
    }

    return isInTransition;
}

void UIInteractable::SetTransitionsInfo(TransitionsInfo info)
{
    CurrentTransitionsInfo = info;
}

TransitionsInfo UIInteractable::GetTransitionsInfo() const
{
    return CurrentTransitionsInfo;
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

    PlayTransition(IsInteractable ? (IsHovered ? CurrentTransitionsInfo.Hovered : CurrentTransitionsInfo.Normal) : CurrentTransitionsInfo.Disabled);
}

bool UIInteractable::GetInteractable() const
{
    return IsInteractable;
}

void UIInteractable::StopTransition()
{
    if (isInTransition)
    {
        isInTransition = false;
        Application::Instance->GetCurrentScene()->GetUILayer()->RemoveFromUpdateQueue(Owner);
    }
}

void UIInteractable::PlayTransition(TransitionData data)
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
    targetTransitionData = data;
    isInTransition = true;
    transitionProgress = 0.0f;
}
