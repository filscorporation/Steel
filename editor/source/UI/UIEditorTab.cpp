#include <Steel/Scene/SceneHelper.h>
#include <Steel/UI/UIElements/UIImage.h>
#include <Steel/UI/UIElements/UITabs.h>
#include "UIEditorTab.h"

#include <Steel/Serialization/AttributesRegistration.h>

void UIEditorTab::RegisterType()
{
    REGISTER_COMPONENT(UIEditorTab);
}

void UIEditorTab::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UIEditorTab::HandleEvent;
    eventHandler.Type = EventHandlerTypes::Transparent;
    eventHandler.EventsMask = UIEventTypes::MouseJustPressedAnywhere;
}

void UIEditorTab::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isFocused = false;
    SetIsFocused(true);
}

void UIEditorTab::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIEditorTab>(handler).HandleEventInner(eventType, uiEvent);
}

void UIEditorTab::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();
    auto& eh = entitiesRegistry->GetComponent<UIEventHandler>(Owner);
    auto& rectTransformation = entitiesRegistry->GetComponent<RectTransformation>(eh.RectEntity);
    bool contains = rectTransformation.Contains(uiEvent.MousePosition);

    if (eventType & UIEventTypes::MouseJustPressedAnywhere)
    {
        SetIsFocused(contains);
    }
}

void UIEditorTab::SetIsFocused(bool value)
{
    if (value == isFocused)
        return;

    isFocused = value;

    if (TabsEntityID != NULL_ENTITY)
    {
        auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();
        auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
        if (!entitiesRegistry->HasComponent<UITabs>(TabsEntityID))
        {
            TabsEntityID = NULL_ENTITY;
            return;
        }
        auto& tabs = entitiesRegistry->GetComponent<UITabs>(TabsEntityID);
        tabs.SetOpenTabSprite(isFocused ? layer->UIResources.DefaultTabFocusedSprite : layer->UIResources.DefaultTabOpenedSprite);
        tabs.SetCloseTabSprite(layer->UIResources.DefaultTabClosedSprite);
    }
}

bool UIEditorTab::GetIsFocused()
{
    return isFocused;
}
