#include "UISystem.h"
#include "UIEventHandler.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Scripting/ScriptingCore.h"

void UISystem::OnComponentAdded(EntityID entityID, UIText& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIText>::Registry, entityID))
    {
        ComponentSystem<UIText>::Registry->RemoveComponent<UIText>(entityID);
        return;
    }

    component._font = Application::Instance->GetResourcesManager()->DefaultFont();
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

void UISystem::OnComponentAdded(EntityID entityID, UIImage& component)
{
    if (!CheckRectTransformation(ComponentSystem<UIImage>::Registry, entityID))
    {
        ComponentSystem<UIImage>::Registry->RemoveComponent<UIImage>(entityID);
        return;
    }

    TryAddEventHandler(ComponentSystem<UIImage>::Registry, entityID);
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

    auto& eh = ComponentSystem<UIButton>::Registry->GetComponent<UIEventHandler>(entityID);

    eh.EventCallback = UIButton::HandleEvent;
    eh.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                    | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;

    component._transitionsInfo.TransitionType = ButtonTransitionTypes::ColorShift;
    component._transitionsInfo.TransitionDuration = 0.3f;
    component._transitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    component._transitionsInfo.Hovered.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    component._transitionsInfo.Clicked.FromColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    component._transitionsInfo.Disabled.FromColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
}

void UISystem::OnComponentRemoved(EntityID entityID, UIButton& component)
{
    if (component.isInTransition)
    {
        component.isInTransition = false;
        Application::Instance->GetCurrentScene()->GetUILayer()->RemoveButtonFromUpdateQueue(entityID);
    }
    ScriptingCore::CallEventMethod(entityID, ScriptingCore::EventManagerCalls.callDeregisterCallbacks);

    ClearRenderers(component);
}

void UISystem::OnEntityEnabled(EntityID entityID, UIButton& component)
{
    RenderersSetActive(component, true);
}

void UISystem::OnEntityDisabled(EntityID entityID, UIButton& component)
{
    RenderersSetActive(component, false);
}

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
    if (entitiesRegistry->HasComponent<UIEventHandler>(entityID))
    {
        Log::LogWarning("Entity {0} already has UIEventHandler component attached, "
                         "this may lead to conflicts. Keep one UIEventHandler per object", entityID);
    }
    else
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
