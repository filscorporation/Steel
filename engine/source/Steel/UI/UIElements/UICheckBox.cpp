#include "UICheckBox.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scripting/ScriptingCore.h"
#include "Steel/Scripting/ScriptingSystem.h"

void UICheckBox::RegisterType()
{
    REGISTER_COMPONENT(UICheckBox);
    REGISTER_ATTRIBUTE(UICheckBox, "value", GetValue, SetValue, bool, AttributeFlags::Public, "Value");
    REGISTER_ATTRIBUTE(UICheckBox, "transitionInfo", GetTransitionsInfo, SetTransitionsInfo, TransitionsInfo, AttributeFlags::Public, "Transition info");
    REGISTER_ID_ATTRIBUTE(UICheckBox, "targetImage", GetTargetImage, SetTargetImage, AttributeFlags::Public, "Target image");
    REGISTER_ATTRIBUTE(UICheckBox, "isInteractable", GetInteractable, SetInteractable, bool, AttributeFlags::Public, "Is interactable");
}

bool UICheckBox::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UICheckBox::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    UIInteractable::SetDefaultTransitionInfo();
}

void UICheckBox::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UICheckBox::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit | UIEventTypes::MouseJustPressed;

    UIInteractable::InitTransitionInfo(UpdateTransition);
}

void UICheckBox::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    StopTransition();
    if (ScriptingSystem::IsInitialized() && Application::Context()->Scripting)
        ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::CheckBoxChangeValue, ScriptingCore::GetDeregisterCallbacksMethod());
}

bool UICheckBox::GetValue() const
{
    return isChecked;
}

void UICheckBox::SetValue(bool value)
{
    if (value == isChecked)
        return;

    isChecked = value;
    if (checkMark != NULL_ENTITY)
    {
        Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(checkMark, isChecked, true);
    }

    if (Callback != nullptr)
        Callback(Owner);
    if (ScriptingSystem::IsInitialized() && Application::Context()->Scripting)
        ScriptingCore::CallCallbackMethod(Owner, CallbackTypes::CheckBoxChangeValue, ScriptingCore::GetInvokeCallbacksMethod());
}

void UICheckBox::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UICheckBox>(handler).HandleEventInner(eventType, uiEvent);
}

bool UICheckBox::UpdateTransition(EntityID entityID)
{
    return GetComponentS<UICheckBox>(entityID).UIInteractable::UpdateTransition();
}

void UICheckBox::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
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
    if (eventType & UIEventTypes::MouseJustPressed)
    {
        SetValue(!isChecked);
    }
}
