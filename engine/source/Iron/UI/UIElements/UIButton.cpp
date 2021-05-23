#include "UIButton.h"
#include "../UIQuadRenderer.h"
#include "../UIEventHandler.h"
#include "../../Core/Log.h"
#include "../../Core/Time.h"
#include "../../Math/Math.h"
#include "../../Scene/SceneHelper.h"
#include "../../Scripting/ScriptingCore.h"
#include "../../Scripting/ScriptingSystem.h"

bool UIButton::UpdateTransition()
{
    transitionProgress += Time::UnscaledDeltaTime();
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_image->IsSliced)
    {
        for (uint32_t _renderer : _renderers)
        {
            auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(_renderer);
            UpdateTransitionForRenderer(qr);
        }
    }
    else
    {
        UpdateTransitionForRenderer(entitiesRegistry->GetComponent<UIQuadRenderer>(Owner));
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
    if (info.TransitionType == ButtonTransitionTypes::SpriteTileChange && _image->IsSliced)
    {
        Log::LogWarning("Tile change transition type is not supported for sliced images");
        return;
    }

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

    targetTransitionData = data;
    isInTransition = true;
    transitionProgress = 0.0f;
}

void UIButton::UpdateTransitionForRenderer(UIQuadRenderer& renderer)
{
    float t = transitionProgress / _transitionsInfo.TransitionDuration;
    switch (_transitionsInfo.TransitionType)
    {
        case ButtonTransitionTypes::ColorShift:
            _color = Math::Lerp(renderer.Color, targetTransitionData.ToColor(), t);
            renderer.Color = Math::Lerp(renderer.Color, targetTransitionData.ToColor(), t);
            break;
        case ButtonTransitionTypes::SpriteChange:
            SetImage(Application::Instance->GetResourcesManager()->GetImage(targetTransitionData.Value));
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case ButtonTransitionTypes::SpriteTileChange:
            _image->GetTexCoord(targetTransitionData.Value, renderer.TextureCoords);
            isInTransition = false;
            transitionProgress = 0.0f;
            break;
        case ButtonTransitionTypes::Animation:
            // TODO: not supported yet
            break;
    }
}
