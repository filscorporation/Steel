#include "EditorScene.h"
#include "../UI/AppView.h"
#include "../UI/SceneView.h"
#include "../UI/HierarchyView.h"
#include "../UI/ProjectView.h"
#include "../UI/PropertiesView.h"
#include "../UI/StatsWindow.h"

void EditorScene::AfterUpdate()
{
    Scene::AfterUpdate();

    auto controlPanels = entitiesRegistry->GetComponentIterator<ControlPanel>();
    for (int i = 0; i < controlPanels.Size(); ++i)
    {
        if (controlPanels[i].IsAlive())
            controlPanels[i].Update(entitiesRegistry);
    }

    auto hierarchyViews = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViews.Size(); ++i)
    {
        if (hierarchyViews[i].IsAlive())
            hierarchyViews[i].Update(entitiesRegistry);
    }

    auto propertiesViews = entitiesRegistry->GetComponentIterator<PropertiesView>();
    for (int i = 0; i < propertiesViews.Size(); ++i)
    {
        if (propertiesViews[i].IsAlive())
            propertiesViews[i].Update(entitiesRegistry);
    }

    auto projectViews = entitiesRegistry->GetComponentIterator<ProjectView>();
    for (int i = 0; i < projectViews.Size(); ++i)
    {
        if (projectViews[i].IsAlive())
            projectViews[i].Update(entitiesRegistry);
    }
}

void EditorScene::BeforeRebuildUI()
{
    Scene::BeforeRebuildUI();

    auto statsWindows = entitiesRegistry->GetComponentIterator<StatsWindow>();
    for (int i = 0; i < statsWindows.Size(); ++i)
    {
        if (statsWindows[i].IsAlive())
            statsWindows[i].UpdateInfo();
    }
}

void EditorScene::AfterPrepareDraw()
{
    Scene::AfterPrepareDraw();

    auto appViews = entitiesRegistry->GetComponentIterator<AppView>();
    for (int i = 0; i < appViews.Size(); ++i)
    {
        if (appViews[i].IsAlive())
            appViews[i].Update(entitiesRegistry);
    }

    auto sceneViews = entitiesRegistry->GetComponentIterator<SceneView>();
    for (int i = 0; i < sceneViews.Size(); ++i)
    {
        if (sceneViews[i].IsAlive())
            sceneViews[i].Update(entitiesRegistry);
    }
}
