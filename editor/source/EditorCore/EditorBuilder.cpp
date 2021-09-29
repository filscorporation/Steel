#include "EditorBuilder.h"
#include "../UI/AppView.h"
#include "../UI/HierarchyView.h"

#include <Steel.h>

#undef LoadImage

void EditorBuilder::BuildLayout(EditorScene* editorScene)
{
    auto entitiesRegistry = editorScene->GetEntitiesRegistry();
    auto uiLayer = editorScene->GetUILayer();

    // Create layout entities1
    EntityID layout1Entity = uiLayer->CreateUILayoutGroup(LayoutGroupTypes::Horizontal, "Layout1", NULL_ENTITY);
    EntityID layout2Entity = uiLayer->CreateUILayoutGroup(LayoutGroupTypes::Vertical, "Layout2", layout1Entity);
    EntityID layout3Entity = uiLayer->CreateUILayoutGroup(LayoutGroupTypes::Horizontal, "Layout2", layout2Entity);

    // Arrange layout
    auto& layout1rt = entitiesRegistry->GetComponent<RectTransformation>(layout1Entity);
    layout1rt.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    auto& layout1 = entitiesRegistry->GetComponent<UILayoutGroup>(layout1Entity);
    layout1.AddElement(layout2Entity);
    LayoutElementInfo info {};
    info.MinWidth = 200;
    info.PreferredWidth = 300;
    info.MinHeight = 400;
    info.PreferredHeight = 1000;
    EntityID l1child1Entity = layout1.AddElement(info);

    auto& layout2 = entitiesRegistry->GetComponent<UILayoutGroup>(layout2Entity);
    info.MinWidth = 400;
    info.PreferredWidth = 600;
    info.MinHeight = 200;
    info.PreferredHeight = 400;
    EntityID l2child1Entity = layout2.AddElement(info);
    layout2.AddElement(layout3Entity);

    auto& layout3 = entitiesRegistry->GetComponent<UILayoutGroup>(layout3Entity);
    info.MinWidth = 200;
    info.PreferredWidth = 300;
    info.MinHeight = 300;
    info.PreferredHeight = 800;
    EntityID l3child1Entity = layout3.AddElement(info);
    info.MinWidth = 800;
    info.PreferredWidth = 2000;
    info.MinHeight = 600;
    info.PreferredHeight = 1000;
    EntityID l3child2Entity = layout3.AddElement(info);

    // Init layout elements

    // Components view
    {
        EntityID componentsTabsEntity = uiLayer->CreateUITabs({"Components"}, "Components tabs", l1child1Entity);
        auto& componentsTabsRT = entitiesRegistry->GetComponent<RectTransformation>(componentsTabsEntity);
        componentsTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        componentsTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& componentsTabs = entitiesRegistry->GetComponent<UITabs>(componentsTabsEntity);

        // Components view
        EntityID componentsViewEntity = uiLayer->CreateUIElement("Components view", componentsTabs.GetTab(0));
        auto& componentsViewRT = entitiesRegistry->GetComponent<RectTransformation>(componentsViewEntity);
        componentsViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        componentsViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& componentsImage = entitiesRegistry->AddComponent<UIImage>(componentsViewEntity);
        componentsImage.SetImage(uiLayer->UIResources.DefaultFrameSprite);
    }

    // Project view
    {
        EntityID projectTabsEntity = uiLayer->CreateUITabs({"Project"}, "Project tabs", l2child1Entity);
        auto& projectTabsRT = entitiesRegistry->GetComponent<RectTransformation>(projectTabsEntity);
        projectTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        projectTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& projectTabs = entitiesRegistry->GetComponent<UITabs>(projectTabsEntity);

        // Project view
        EntityID projectViewEntity = uiLayer->CreateUIElement("Project view", projectTabs.GetTab(0));
        auto& projectViewRT = entitiesRegistry->GetComponent<RectTransformation>(projectViewEntity);
        projectViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        projectViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& projectImage = entitiesRegistry->AddComponent<UIImage>(projectViewEntity);
        projectImage.SetImage(uiLayer->UIResources.DefaultFrameSprite);
    }

    // Hierarchy view
    {
        EntityID hierarchyTabsEntity = uiLayer->CreateUITabs({"Hierarchy"}, "Hierarchy tabs", l3child1Entity);
        auto& hierarchyTabsRT = entitiesRegistry->GetComponent<RectTransformation>(hierarchyTabsEntity);
        hierarchyTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        hierarchyTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& hierarchyTabs = entitiesRegistry->GetComponent<UITabs>(hierarchyTabsEntity);

        // Hierarchy view
        EntityID hierarchyViewEntity = uiLayer->CreateUIElement("Hierarchy view", hierarchyTabs.GetTab(0));
        auto& hierarchyViewRT = entitiesRegistry->GetComponent<RectTransformation>(hierarchyViewEntity);
        hierarchyViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        hierarchyViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& hierarchyView = entitiesRegistry->AddComponent<HierarchyView>(hierarchyViewEntity);
        hierarchyView.Init(entitiesRegistry);
    }

    // Application and scene views
    {
        EntityID appSceneTabsEntity = uiLayer->CreateUITabs({"Application", "Scene"}, "Application tabs", l3child2Entity);
        auto& appSceneTabsRT = entitiesRegistry->GetComponent<RectTransformation>(appSceneTabsEntity);
        appSceneTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        appSceneTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& appSceneTabs = entitiesRegistry->GetComponent<UITabs>(appSceneTabsEntity);

        // Application view
        EntityID appViewEntity = uiLayer->CreateUIElement("App view", appSceneTabs.GetTab(0));
        auto& appViewRT = entitiesRegistry->GetComponent<RectTransformation>(appViewEntity);
        appViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        appViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& appView = entitiesRegistry->AddComponent<AppView>(appViewEntity);
        appView.Init(entitiesRegistry);
    }
}
