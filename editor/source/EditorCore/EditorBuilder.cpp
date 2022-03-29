#include "EditorBuilder.h"
#include "../UI/AppView.h"
#include "../UI/ControlPanel.h"
#include "../UI/HierarchyView.h"
#include "../UI/PropertiesView.h"
#include "../UI/UIEditorTab.h"
#include "../UI/SceneView.h"

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

    // Control panel
    {
        EntityID cpEntity = uiLayer->CreateUIElement("Control panel", NULL_ENTITY);
        auto& cpRT = entitiesRegistry->GetComponent<RectTransformation>(cpEntity);
        cpRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        cpRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        cpRT.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_BUTTON_H * 1.8f * 0.5f));
        cpRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H * 1.8f));
        auto& controlPanel = entitiesRegistry->AddComponent<ControlPanel>(cpEntity);
        controlPanel.Init();
    }

    // Arrange layout
    auto& layout1rt = entitiesRegistry->GetComponent<RectTransformation>(layout1Entity);
    layout1rt.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    layout1rt.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    layout1rt.SetOffsetMax(glm::vec2(0.0f, STYLE_BUTTON_H * 1.8f));
    auto& layout1 = entitiesRegistry->GetComponent<UILayoutGroup>(layout1Entity);
    layout1.AddElement(layout2Entity);
    EntityID l1child1Entity = layout1.AddElement(200, 400, 300, 1000);

    auto& layout2 = entitiesRegistry->GetComponent<UILayoutGroup>(layout2Entity);
    EntityID l2child1Entity = layout2.AddElement(400, 200, 600, 400);
    layout2.AddElement(layout3Entity);

    auto& layout3 = entitiesRegistry->GetComponent<UILayoutGroup>(layout3Entity);
    EntityID l3child1Entity = layout3.AddElement(200, 300, 300, 800);
    EntityID l3child2Entity = layout3.AddElement(800, 600, 2000, 1000);

    // Init layout elements

    // Properties view
    {
        EntityID propertiesTabsEntity = uiLayer->CreateUITabs({"Properties"}, "Properties tabs", l1child1Entity);
        auto& propertiesTabsRT = entitiesRegistry->GetComponent<RectTransformation>(propertiesTabsEntity);
        propertiesTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        propertiesTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& propertiesTabs = entitiesRegistry->GetComponent<UITabs>(propertiesTabsEntity);

        // Properties view
        entitiesRegistry->AddComponent<UIEditorTab>(propertiesTabs.GetTab(0)).TabsEntityID = propertiesTabsEntity;
        entitiesRegistry->GetComponent<UIEventHandler>(propertiesTabs.GetTab(0)).RectEntity = l1child1Entity;
        EntityID propertiesViewEntity = uiLayer->CreateUIElement("Properties view", propertiesTabs.GetTab(0));
        auto& propertiesViewRT = entitiesRegistry->GetComponent<RectTransformation>(propertiesViewEntity);
        propertiesViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        propertiesViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& propertiesView = entitiesRegistry->AddComponent<PropertiesView>(propertiesViewEntity);
        propertiesView.Init(entitiesRegistry);
    }

    // Project view
    {
        EntityID projectTabsEntity = uiLayer->CreateUITabs({"Project"}, "Project tabs", l2child1Entity);
        auto& projectTabsRT = entitiesRegistry->GetComponent<RectTransformation>(projectTabsEntity);
        projectTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        projectTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& projectTabs = entitiesRegistry->GetComponent<UITabs>(projectTabsEntity);

        // Project view
        entitiesRegistry->AddComponent<UIEditorTab>(projectTabs.GetTab(0)).TabsEntityID = projectTabsEntity;
        entitiesRegistry->GetComponent<UIEventHandler>(projectTabs.GetTab(0)).RectEntity = l2child1Entity;
        EntityID projectViewEntity = uiLayer->CreateUIElement("Project view", projectTabs.GetTab(0));
        auto& projectViewRT = entitiesRegistry->GetComponent<RectTransformation>(projectViewEntity);
        projectViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        projectViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& projectImage = entitiesRegistry->AddComponent<UIImage>(projectViewEntity);
        projectImage.SetImage(uiLayer->UIResources.StraightFrameSprite);
    }

    // Hierarchy view
    {
        EntityID hierarchyTabsEntity = uiLayer->CreateUITabs({"Hierarchy"}, "Hierarchy tabs", l3child1Entity);
        auto& hierarchyTabsRT = entitiesRegistry->GetComponent<RectTransformation>(hierarchyTabsEntity);
        hierarchyTabsRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        hierarchyTabsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& hierarchyTabs = entitiesRegistry->GetComponent<UITabs>(hierarchyTabsEntity);

        // Hierarchy view
        entitiesRegistry->AddComponent<UIEditorTab>(hierarchyTabs.GetTab(0)).TabsEntityID = hierarchyTabsEntity;
        entitiesRegistry->GetComponent<UIEventHandler>(hierarchyTabs.GetTab(0)).RectEntity = l3child1Entity;
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
        EntityID appTabEntity = appSceneTabs.GetTab(0);
        entitiesRegistry->AddComponent<UIEditorTab>(appTabEntity).TabsEntityID = appSceneTabsEntity;
        entitiesRegistry->GetComponent<UIEventHandler>(appTabEntity).RectEntity = l3child2Entity;
        EntityID appViewEntity = uiLayer->CreateUIElement("App view", appTabEntity);
        auto& appViewRT = entitiesRegistry->GetComponent<RectTransformation>(appViewEntity);
        appViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        appViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& appView = entitiesRegistry->AddComponent<AppView>(appViewEntity);
        appView.Init(entitiesRegistry);

        // Scene view
        EntityID sceneTabEntity = appSceneTabs.GetTab(1);
        entitiesRegistry->AddComponent<UIEditorTab>(sceneTabEntity).TabsEntityID = appSceneTabsEntity;
        entitiesRegistry->GetComponent<UIEventHandler>(sceneTabEntity).RectEntity = l3child2Entity;
        EntityID sceneViewEntity = uiLayer->CreateUIElement("Scene view", sceneTabEntity);
        auto& sceneViewRT = entitiesRegistry->GetComponent<RectTransformation>(sceneViewEntity);
        sceneViewRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        sceneViewRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        auto& sceneView = entitiesRegistry->AddComponent<SceneView>(sceneViewEntity);
        sceneView.Init(entitiesRegistry);
    }
}
