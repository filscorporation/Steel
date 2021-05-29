#include "UIButton.h"
#include "../UIQuadRenderer.h"
#include "../UIEventHandler.h"
#include "../../Core/Log.h"
#include "../../Core/Time.h"
#include "../../Math/Math.h"
#include "../../Scene/SceneHelper.h"
#include "../../Scripting/ScriptingCore.h"
#include "../../Scripting/ScriptingSystem.h"

void UIButton::Init(UIEventHandler& eventHandler)
{
    eventHandler.EventCallback = UIButton::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                    | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    _transitionsInfo.TransitionType = ButtonTransitionTypes::ColorShift;
    _transitionsInfo.TransitionDuration = 0.1f;
    _transitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    _transitionsInfo.Hovered.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    _transitionsInfo.Clicked.FromColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    _transitionsInfo.Disabled.FromColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
}

bool UIButton::UpdateTransition()
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

    float t = transitionProgress / _transitionsInfo.TransitionDuration;
    switch (_transitionsInfo.TransitionType)
    {
        case ButtonTransitionTypes::ColorShift:
            if (needInit)
                startingTransitionData.FromColor(image.GetColor());
            image.SetColor(Math::Lerp(startingTransitionData.ToColor(), targetTransitionData.ToColor(), t));
            break;
        case ButtonTransitionTypes::SpriteChange:
            image.SetImage(Application::Instance->GetResourcesManager()->GetImage(targetTransitionData.Value));
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case ButtonTransitionTypes::SpriteTileChange:
            image.SetImageTileIndex(targetTransitionData.Value);
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case ButtonTransitionTypes::Animation:
            // TODO: not supported yet
            break;
    }

    if (transitionProgress > _transitionsInfo.TransitionDuration)
    {
        isInTransition = false;
        transitionProgress = 0.0f;
    }

    return isInTransition;
}

void UIButton::SetTransitionsInfo(ButtonTransitionsInfo info)
{
    _transitionsInfo = info;
}

ButtonTransitionsInfo UIButton::GetTransitionsInfo() const
{
    return _transitionsInfo;
}

void UIButton::SetTargetImage(EntityID targetID)
{
    _targetImage = targetID;
}

EntityID UIButton::GetTargetImage() const
{
    return _targetImage;
}

void UIButton::SetInteractable(bool interactable)
{
    if (_interactable == interactable)
        return;

    _interactable = interactable;
    isHovered = false;

    PlayTransition(_interactable ? (isHovered ? _transitionsInfo.Hovered : _transitionsInfo.Normal) : _transitionsInfo.Disabled);
}

bool UIButton::GetInteractable() const
{
    return _interactable;
}

void UIButton::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIButton>(handler).HandleEventInner(eventType, uiEvent);
}

void UIButton::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (!_interactable)
        return;

    if (eventType & UIEventTypes::MouseEnter)
    {
        isHovered = true;
        PlayTransition(_transitionsInfo.Hovered);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        isHovered = false;
        PlayTransition(_transitionsInfo.Normal);
    }
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        PlayTransition(_transitionsInfo.Clicked);
        if (Callback != nullptr)
            Callback(Owner);
        if (ScriptingSystem::IsInitialized())
            ScriptingCore::CallEventMethod(Owner, ScriptingCore::EventManagerCalls.callInvokeCallbacks);
    }
    if (eventType & UIEventTypes::MouseJustReleased)
    {
        PlayTransition(_transitionsInfo.Hovered);
    }
}

void UIButton::PlayTransition(ButtonTransitionData data)
{
    if (isInTransition)
    {
        transitionProgress = 1.0f - transitionProgress;
    }
    else
    {
        Application::Instance->GetCurrentScene()->GetUILayer()->AddButtonToUpdateQueue(Owner);
    }

    startingDataInitialized = false;
    targetTransitionData = data;
    isInTransition = true;
    transitionProgress = 0.0f;
}
