#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UIEventHandler.h>

class UIEditorTab : public UIComponent
{
    DEFINE_TYPE(UIEditorTab)

public:
    explicit UIEditorTab(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    static void HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent);
    void HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent);

    void SetIsFocused(bool value);
    bool GetIsFocused();

    EntityID TabsEntityID = NULL_ENTITY;

private:
    bool isFocused = false;
};
