#pragma once

#include "UITabInfo.h"
#include "Steel/UI/UIComponent.h"
#include "Steel/UI/RectTransformation.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"

class UITabs : public UIComponent
{
    DEFINE_TYPE(UITabs)

public:
    explicit UITabs(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;

    void SetOpenTabSprite(Sprite* tabOpenedSprite);
    Sprite* GetOpenTabSprite() const;
    void SetCloseTabSprite(Sprite* tabClosedSprite);
    Sprite* GetCloseTabSprite() const;

    EntityID GetTab(int index);
    int GetTabsCount();
    EntityID AddTab(const std::string& name);
    bool RemoveTab(int index);
    int GetActiveTab() const;
    void SetActiveTab(int index);

private:
    void SetActiveTabByButtonID(EntityID buttonID);
    void SetButtonRect(RectTransformation& buttonRT, int index) const;

    const std::vector<UITabInfo>& GetTabsList() const;
    void SetTabsList(const std::vector<UITabInfo>& tabs);
    void ClearTabs();

    EntityID GetContent() const;
    void SetContent(EntityID contentEntityID);

    bool initialized = false;
    std::vector<UITabInfo> _tabs;
    EntityID content = NULL_ENTITY;
    int activeTab = -1;
    int _activeTabBackup = -1;

    uint32_t buttonHeight = 0;
    uint32_t buttonWidth = 0;
    Sprite* _tabOpenedSprite = nullptr;
    Sprite* _tabClosedSprite = nullptr;
};
