#include "UIButton.h"
#include "UIEventHandler.h"
#include "../Core/Time.h"
#include "../Math/Math.h"
#include "../Scene/SceneHelper.h"
#include "../Scripting/ScriptingCore.h"

UIButton::UIButton(EntityID ownerEntityID) : UIImage(ownerEntityID)
{
    auto& eh = GetComponentS<UIEventHandler>(ownerEntityID);

    eh.EventCallback = HandleEvent;
    eh.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                  | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    _transitionsInfo.TransitionType = ButtonTransitionTypes::ColorShift;
    _transitionsInfo.TransitionDuration = 0.3f;
    _transitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    _transitionsInfo.Hovered.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    _transitionsInfo.Clicked.FromColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    _transitionsInfo.Disabled.FromColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
}

UIButton::~UIButton()
{
    if (isInTransition)
    {
        isInTransition = false;
        Application::Instance->GetCurrentScene()->GetUILayer()->RemoveButtonFromUpdateQueue(Owner);
    }
}

bool UIButton::Update()
{
    transitionProgress += Time::DeltaTime();
    float t = transitionProgress / _transitionsInfo.TransitionDuration;
    auto& uii = GetComponentS<UIRenderer>(Owner);
    switch (_transitionsInfo.TransitionType)
    {
        case ButtonTransitionTypes::ColorShift:
            uii.Color = Math::Lerp(uii.Color, targetTransitionData.ToColor(), t);
            break;
        case ButtonTransitionTypes::SpriteChange:
            SetImage(Application::Instance->GetResourcesManager()->GetImage(targetTransitionData.Value));
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case ButtonTransitionTypes::SpriteTileChange:
            _image->GetTexCoord(targetTransitionData.Value, uii.TextureCoords);
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

    targetTransitionData = data;
    isInTransition = true;
    transitionProgress = 0.0f;
}
