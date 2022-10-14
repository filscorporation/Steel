#include "UIButton.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scripting/ScriptingCore.h"
#include "Steel/Scripting/ScriptingSystem.h"

void UIButton::RegisterType()
{
    REGISTER_COMPONENT(UIButton);
    REGISTER_ATTRIBUTE(UIButton, "transitionInfo", GetTransitionsInfo, SetTransitionsInfo, TransitionsInfo, AttributeFlags::Public, "Transition info");
    REGISTER_ID_ATTRIBUTE(UIButton, "targetImage", GetTargetImage, SetTargetImage, AttributeFlags::Public, "Target image");
    REGISTER_ATTRIBUTE(UIButton, "isInteractable", GetInteractable, SetInteractable, bool, AttributeFlags::Public, "Is interactable");
}

bool UIButton::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIButton::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    UIInteractable::SetDefaultTransitionInfo();
}

void UIButton::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UIButton::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                              | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    UIInteractable::InitTransitionInfo(UpdateTransition);
}

void UIButton::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    StopTransition();
    if (ScriptingSystem::IsInitialized() && Application::Context()->Scripting)
        ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::ButtonClick, ScriptingCore::GetDeregisterCallbacksMethod());
}

void UIButton::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    RestoreTransition();
}

void UIButton::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIButton>(handler).HandleEventInner(eventType, uiEvent);
}

bool UIButton::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UIButton>(entityID).UIInteractable::UpdateTransition();
}

void UIButton::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (!IsInteractable)
        return;

    if (eventType & UIEventTypes::MouseEnter)
    {
        IsHovered = true;
        PlayTransition(TransitionStates::Hovered);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        IsHovered = false;
        PlayTransition(TransitionStates::Normal);
    }
    if (eventType & UIEventTypes::MouseJustReleased)
    {
        PlayTransition(TransitionStates::Hovered);
    }
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        PlayTransition(TransitionStates::Clicked);
        EntityID ownerBackup = Owner;
        if (Callback != nullptr)
            Callback(ownerBackup);
        if (ScriptingSystem::IsInitialized() && Application::Context()->Scripting)
            ScriptingCore::CallCallbackMethod(ownerBackup, CallbackTypes::ButtonClick, ScriptingCore::GetInvokeCallbacksMethod());
    }
}
