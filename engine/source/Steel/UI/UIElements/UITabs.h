#pragma once

#include "../UIComponent.h"
#include "../RectTransformation.h"
#include "../../EntityComponentSystem/EntitiesRegistry.h"

struct UITabInfo
{
    UITabInfo(EntityID buttonID, EntityID contentID)
    {
        ButtonID = buttonID;
        ContentID = contentID;
    }

    EntityID ButtonID;
    EntityID ContentID;
};

class UITabs : public UIComponent
{
public:
    explicit UITabs(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;

    void SetTabsSprites(Sprite* tabOpenedSprite, Sprite* tabClosedSprite);

    EntityID GetTab(int index);
    int GetTabsCount();
    EntityID AddTab(const std::string& name);
    bool RemoveTab(int index);
    int GetActiveTab() const;
    void SetActiveTab(int index);

private:
    std::vector<UITabInfo> tabs;
    EntityID content = NULL_ENTITY;
    int activeTab = -1;

    uint32_t buttonHeight = 0;
    uint32_t buttonWidth = 0;
    Sprite* _tabOpenedSprite = nullptr;
    Sprite* _tabClosedSprite = nullptr;

    void SetActiveTabByButtonID(EntityID buttonID);
    void SetButtonRect(RectTransformation& buttonRT, int index);
};
