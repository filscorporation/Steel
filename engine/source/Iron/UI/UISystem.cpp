#include "UISystem.h"
#include "UIEventHandler.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Scripting/ScriptingCore.h"

void UISystem::OnComponentAdded(EntityID entityID, UIImage& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIImage>::Registry, entityID))
    {
        ComponentSystem<UIImage>::Registry->RemoveComponent<UIImage>(entityID);
        return;
    }

    TryAddEventHandler(ComponentSystem<UIImage>::Registry, entityID);
    component.Init(ComponentSystem<UIImage>::Registry);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIImage& component)
{
    ClearRenderers(component);
}

void UISystem::OnEntityEnabled(EntityID entityID, UIImage& component)
{
    RenderersSetActive(component, true);
}

void UISystem::OnEntityDisabled(EntityID entityID, UIImage& component)
{
    RenderersSetActive(component, false);
}

void UISystem::OnComponentAdded(EntityID entityID, UIButton& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIButton>::Registry, entityID))
    {
        ComponentSystem<UIButton>::Registry->RemoveComponent<UIButton>(entityID);
        return;
    }
    TryAddEventHandler(ComponentSystem<UIButton>::Registry, entityID);

    auto& eventHandler = ComponentSystem<UIButton>::Registry->GetComponent<UIEventHandler>(entityID);
    component.Init(eventHandler);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIButton& component)
{
    component.StopTransition();
    ScriptingCore::CallEventMethod(entityID, CallbackTypes::ButtonClick, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
}

void UISystem::OnEntityEnabled(EntityID entityID, UIButton& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UIButton& component)
{ }

void UISystem::OnComponentAdded(EntityID entityID, UIText& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIText>::Registry, entityID))
    {
        ComponentSystem<UIText>::Registry->RemoveComponent<UIText>(entityID);
        return;
    }

    component._font = Application::Instance->GetResourcesManager()->DefaultFont();
    component.Init(ComponentSystem<UIText>::Registry);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIText& component)
{
    component.ForeachLetterDelete(ComponentSystem<UIText>::Registry, component.letters.size());
}

void UISystem::OnEntityEnabled(EntityID entityID, UIText& component)
{
    component.ForeachLetterSetActive(ComponentSystem<UIText>::Registry, true);
}

void UISystem::OnEntityDisabled(EntityID entityID, UIText& component)
{
    component.ForeachLetterSetActive(ComponentSystem<UIText>::Registry, false);
}

void UISystem::OnComponentAdded(EntityID entityID, UIInputField& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIInputField>::Registry, entityID))
    {
        ComponentSystem<UIInputField>::Registry->RemoveComponent<UIInputField>(entityID);
        return;
    }

    TryAddEventHandler(ComponentSystem<UIInputField>::Registry, entityID);

    auto& eventHandler = ComponentSystem<UIInputField>::Registry->GetComponent<UIEventHandler>(entityID);
    component.Init(eventHandler);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIInputField& component)
{
    component.OnRemoved();
    component.StopTransition();
    ScriptingCore::CallEventMethod(entityID, CallbackTypes::InputFieldChangeValue, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
    ScriptingCore::CallEventMethod(entityID, CallbackTypes::InputFieldEndEdit, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
}

void UISystem::OnEntityEnabled(EntityID entityID, UIInputField& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UIInputField& component)
{
    component.OnDisabled();
}

void UISystem::OnComponentAdded(EntityID entityID, UIClipping& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIClipping>::Registry, entityID))
    {
        ComponentSystem<UIClipping>::Registry->RemoveComponent<UIClipping>(entityID);
        return;
    }

    component.Init(ComponentSystem<UIClipping>::Registry);
}

void UISystem::OnComponentRemoved(EntityID entityID, UIClipping& component)
{
    component.OnRemoved(ComponentSystem<UIClipping>::Registry);
}

void UISystem::OnEntityEnabled(EntityID entityID, UIClipping& component)
{
    component.OnEnabled(ComponentSystem<UIClipping>::Registry);
}

void UISystem::OnEntityDisabled(EntityID entityID, UIClipping& component)
{
    component.OnDisabled(ComponentSystem<UIClipping>::Registry);
}

void UISystem::OnComponentAdded(EntityID entityID, UICheckBox& component)
{
    if (!CheckRectTransformation(ComponentSystem<UICheckBox>::Registry, entityID))
    {
        ComponentSystem<UICheckBox>::Registry->RemoveComponent<UICheckBox>(entityID);
        return;
    }
    TryAddEventHandler(ComponentSystem<UICheckBox>::Registry, entityID);

    auto& eventHandler = ComponentSystem<UICheckBox>::Registry->GetComponent<UIEventHandler>(entityID);
    component.Init(eventHandler);
}

void UISystem::OnComponentRemoved(EntityID entityID, UICheckBox& component)
{
    component.StopTransition();
    ScriptingCore::CallEventMethod(entityID, CallbackTypes::CheckBoxChangeValue, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);
}

void UISystem::OnEntityEnabled(EntityID entityID, UICheckBox& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UICheckBox& component)
{ }

void UISystem::OnComponentAdded(EntityID entityID, UITabs& component)
{
    if (!CheckRectTransformation(ComponentSystem<UITabs>::Registry, entityID))
    {
        ComponentSystem<UITabs>::Registry->RemoveComponent<UITabs>(entityID);
        return;
    }
    component.Init(ComponentSystem<UITabs>::Registry);
}

void UISystem::OnComponentRemoved(EntityID entityID, UITabs& component)
{ }

void UISystem::OnEntityEnabled(EntityID entityID, UITabs& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UITabs& component)
{ }

void UISystem::OnComponentAdded(EntityID entityID, UILayoutGroup& component)
{
    if (ComponentSystem<UILayoutGroup>::Registry->HasComponent<UILayoutElement>(entityID))
    {
        Log::LogWarning("UILayoutElement component will be replaced with UILayoutGroup");
        ComponentSystem<UILayoutGroup>::Registry->RemoveComponent<UILayoutElement>(entityID);
    }
    if (!CheckRectTransformation(ComponentSystem<UILayoutGroup>::Registry, entityID))
    {
        ComponentSystem<UILayoutGroup>::Registry->RemoveComponent<UILayoutGroup>(entityID);
        return;
    }
    TryAddEventHandler(ComponentSystem<UICheckBox>::Registry, entityID);

    auto& eventHandler = ComponentSystem<UICheckBox>::Registry->GetComponent<UIEventHandler>(entityID);
    component.Init(eventHandler);
}

void UISystem::OnComponentRemoved(EntityID entityID, UILayoutGroup& component)
{ }

void UISystem::OnEntityEnabled(EntityID entityID, UILayoutGroup& component)
{ }

void UISystem::OnEntityDisabled(EntityID entityID, UILayoutGroup& component)
{ }

bool UISystem::CheckRectTransformation(EntitiesRegistry* entitiesRegistry, EntityID entityID)
{
    if (!entitiesRegistry->HasComponent<RectTransformation>(entityID))
    {
        Log::LogError("Adding UIComponents to objects without RectTransformation will lead to undefined behaviour. "
                      "Create UI objects using UILayout, not Scene");

        return false;
    }

    return true;
}

void UISystem::TryAddEventHandler(EntitiesRegistry* entitiesRegistry, EntityID entityID)
{
    if (!entitiesRegistry->HasComponent<UIEventHandler>(entityID))
    {
        entitiesRegistry->AddComponent<UIEventHandler>(entityID);
    }
}

void UISystem::ClearRenderers(UIImage& image)
{
    for (auto qrID : image._renderers)
        ComponentSystem<UIButton>::Registry->DeleteEntity(qrID);
    image._renderers.clear();
}

void UISystem::RenderersSetActive(const UIImage& image, bool active)
{
    for (auto qrID : image._renderers)
        ComponentSystem<UIButton>::Registry->EntitySetActive(qrID, active, false);
}
