#include "UITabs.h"
#include "UIButton.h"
#include "UIText.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Serialization/AttributesRegistration.h"

void UITabs::RegisterType()
{
    REGISTER_COMPONENT(UITabs);
    REGISTER_LIST_ATTRIBUTE(UITabs, "tabs", GetTabsList, SetTabsList, UITabInfo, AttributeFlags::Public, "Tabs");
    REGISTER_ATTRIBUTE(UITabs, "activeTab", GetActiveTab, SetActiveTab, int, AttributeFlags::Public, "Active tab");
    REGISTER_ID_ATTRIBUTE(UITabs, "content", GetContent, SetContent, AttributeFlags::Public, "Content");
    REGISTER_RESOURCE_ATTRIBUTE(UITabs, "openTabSprite", GetOpenTabSprite, SetOpenTabSprite, Sprite*, ResourceTypes::Sprite, AttributeFlags::Public, "Opened tab sprite");
    REGISTER_RESOURCE_ATTRIBUTE(UITabs, "closeTabSprite", GetCloseTabSprite, SetCloseTabSprite, Sprite*, ResourceTypes::Sprite, AttributeFlags::Public, "Closed tab sprite");
}

bool UITabs::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UITabs::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();
    _tabOpenedSprite = layer->UIResources.DefaultTabOpenedSprite;
    _tabClosedSprite = layer->UIResources.DefaultTabClosedSprite;

    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    // Set default size
    rt.SetSize(glm::vec2(buttonWidth * 4, buttonWidth * 8));
}

void UITabs::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    initialized = true;
    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();

    buttonWidth = _tabOpenedSprite == nullptr ? 0 : _tabOpenedSprite->GetSpriteTexture()->GetWidth();
    buttonHeight = _tabOpenedSprite == nullptr ? 0 : _tabOpenedSprite->GetSpriteTexture()->GetHeight();

    if (content == NULL_ENTITY)
    {
        content = layer->CreateUIElement("Tabs content", Owner);
        auto& contentRT = entitiesRegistry->GetComponent<RectTransformation>(content);
        contentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        contentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        contentRT.SetOffsetMax(glm::vec2(0.0f, buttonHeight));
    }

    for (auto tab : _tabs)
    {
        auto& button = entitiesRegistry->GetComponent<UIButton>(tab.ButtonID);
        EntityID thisEntityID = Owner;
        button.Callback = [thisEntityID](EntityID entityID)
        {
            auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
            if (registry->HasComponent<UITabs>(thisEntityID))
                registry->GetComponent<UITabs>(thisEntityID).SetActiveTabByButtonID(entityID);
        };
    }

    if (_activeTabBackup != -1)
    {
        SetActiveTab(_activeTabBackup);
        _activeTabBackup = -1;
    }
}

void UITabs::SetOpenTabSprite(Sprite* tabOpenedSprite)
{
    _tabOpenedSprite = tabOpenedSprite;
    buttonWidth = _tabOpenedSprite == nullptr ? 0 : _tabOpenedSprite->GetSpriteTexture()->GetWidth();
    buttonHeight = _tabOpenedSprite == nullptr ? 0 : _tabOpenedSprite->GetSpriteTexture()->GetHeight();

    if (initialized)
    {
        auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

        for (int i = 0; i < _tabs.size(); ++i)
        {
            auto& currentButton = entitiesRegistry->GetComponent<UIButton>(_tabs[i].ButtonID);
            auto& currentButtonSprite = entitiesRegistry->GetComponent<UIImage>(currentButton.GetTargetImage());
            if (activeTab == i)
                currentButtonSprite.SetImage(_tabOpenedSprite);
            else
                currentButtonSprite.SetImage(_tabClosedSprite);
        }
    }
}

Sprite* UITabs::GetOpenTabSprite() const
{
    return _tabOpenedSprite;
}

void UITabs::SetCloseTabSprite(Sprite* tabClosedSprite)
{
    _tabClosedSprite = tabClosedSprite;

    if (initialized)
    {
        auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

        for (int i = 0; i < _tabs.size(); ++i)
        {
            auto& currentButton = entitiesRegistry->GetComponent<UIButton>(_tabs[i].ButtonID);
            auto& currentButtonSprite = entitiesRegistry->GetComponent<UIImage>(currentButton.GetTargetImage());
            if (activeTab == i)
                currentButtonSprite.SetImage(_tabOpenedSprite);
            else
                currentButtonSprite.SetImage(_tabClosedSprite);
        }
    }
}

Sprite* UITabs::GetCloseTabSprite() const
{
    return _tabClosedSprite;
}

EntityID UITabs::GetTab(int index)
{
    if (index < 0 || index >= _tabs.size())
    {
        Log::LogError("Tab index outside of range");
        return NULL_ENTITY;
    }

    return _tabs[index].ContentID;
}

int UITabs::GetTabsCount()
{
    return (int)_tabs.size();
}

EntityID UITabs::AddTab(const std::string& name)
{
    if (content == NULL_ENTITY)
    {
        Log::LogError("Tabs {0} content is null", Owner);
        return NULL_ENTITY;
    }

    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    int index = (int)_tabs.size();

    EntityID newContentEntity = layer->CreateUIElement(name + " tab", content);
    auto& tabRT = entitiesRegistry->GetComponent<RectTransformation>(newContentEntity);
    tabRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    tabRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    EntityID buttonEntity = layer->CreateUIButton(_tabClosedSprite, name + " tab", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(buttonEntity);
    SetButtonRect(buttonRT, index);
    auto& button = entitiesRegistry->GetComponent<UIButton>(buttonEntity);
    EntityID thisEntityID = Owner;
    button.Callback = [thisEntityID](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        if (registry->HasComponent<UITabs>(thisEntityID))
            registry->GetComponent<UITabs>(thisEntityID).SetActiveTabByButtonID(entityID);
    };

    EntityID textEntity = layer->CreateUIText(name, "Label", buttonEntity);
    auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(4.0f, 2.0f));
    textRT.SetOffsetMax(glm::vec2(4.0f, 2.0f));
    auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
    text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _tabs.emplace_back(buttonEntity, newContentEntity);

    entitiesRegistry->EntitySetActive(newContentEntity, false, true);
    if (activeTab == -1)
        SetActiveTab((int)index);

    return newContentEntity;
}

bool UITabs::RemoveTab(int index)
{
    if (index < 0 || index >= _tabs.size())
    {
        Log::LogError("Tab index outside of range");
        return false;
    }

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    entitiesRegistry->DeleteEntity(_tabs[index].ContentID);
    entitiesRegistry->DeleteEntity(_tabs[index].ButtonID);

    _tabs.erase(_tabs.begin() + index);
    for (int i = index; i < _tabs.size(); ++i)
    {
        SetButtonRect(entitiesRegistry->GetComponent<RectTransformation>(_tabs[i].ButtonID), i);
    }

    if (activeTab == index)
    {
        activeTab = -1;
        if (!_tabs.empty())
            SetActiveTab(0);
    }
    else if (activeTab > index)
        activeTab--;

    return true;
}

int UITabs::GetActiveTab() const
{
    return activeTab;
}

void UITabs::SetActiveTab(int index)
{
    if (index == activeTab)
        return;

    if (!initialized)
    {
        // For cases when active tab is set before tabs
        _activeTabBackup = index;
        return;
    }
    _activeTabBackup = -1;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    EntityID contentToDeactivate = NULL_ENTITY;

    if (activeTab != -1)
    {
        contentToDeactivate = _tabs[activeTab].ContentID;
        if (entitiesRegistry->EntityExists(contentToDeactivate))
        {
            auto& lastButton = entitiesRegistry->GetComponent<UIButton>(_tabs[activeTab].ButtonID);
            auto& lastButtonSprite = entitiesRegistry->GetComponent<UIImage>(lastButton.GetTargetImage());
            lastButtonSprite.SetImage(_tabClosedSprite);
            lastButton.StopTransition();
        }
    }

    activeTab = index;
    if (entitiesRegistry->EntityExists(_tabs[activeTab].ContentID))
    {
        auto& currentButton = entitiesRegistry->GetComponent<UIButton>(_tabs[activeTab].ButtonID);
        auto& currentButtonSprite = entitiesRegistry->GetComponent<UIImage>(currentButton.GetTargetImage());
        currentButtonSprite.SetImage(_tabOpenedSprite);
        currentButton.StopTransition();
        entitiesRegistry->EntitySetActive(_tabs[activeTab].ContentID, true, true);
    }

    if (entitiesRegistry->EntityExists(contentToDeactivate))
        entitiesRegistry->EntitySetActive(contentToDeactivate, false, true);
}

void UITabs::SetActiveTabByButtonID(EntityID buttonID)
{
    for (int i = 0; i < _tabs.size(); ++i)
    {
        if (_tabs[i].ButtonID == buttonID)
        {
            SetActiveTab(i);
            return;
        }
    }
}

void UITabs::SetButtonRect(RectTransformation& buttonRT, int index) const
{
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
    buttonRT.SetPivot(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchoredPosition(glm::vec2(index * buttonWidth, 0.0f));
    buttonRT.SetSize(glm::vec2(buttonWidth, buttonHeight));
}

const std::vector<UITabInfo>& UITabs::GetTabsList() const
{
    return _tabs;
}

void UITabs::SetTabsList(const std::vector<UITabInfo>& tabs)
{
    if (!_tabs.empty())
    {
        ClearTabs();
    }

    _tabs = tabs;
}

void UITabs::ClearTabs()
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    for (auto tab : _tabs)
    {
        entitiesRegistry->DeleteEntity(tab.ContentID);
        entitiesRegistry->DeleteEntity(tab.ButtonID);
    }

    _tabs.clear();

    activeTab = -1;
}

EntityID UITabs::GetContent() const
{
    return content;
}

void UITabs::SetContent(EntityID contentEntityID)
{
    content = contentEntityID;
}
