#include "EditorScene.h"
#include "../UI/AppView.h"
#include "../UI/SceneView.h"
#include "../UI/HierarchyView.h"
#include "../UI/StatsWindow.h"

void EditorScene::AfterUpdate()
{
    Scene::AfterUpdate();

    auto controlPanels = entitiesRegistry->GetComponentIterator<ControlPanel>();
    for (int i = 0; i < controlPanels.Size(); ++i)
    {
        controlPanels[i].Update(entitiesRegistry);
    }

    auto hierarchyViews = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViews.Size(); ++i)
    {
        hierarchyViews[i].Update(entitiesRegistry);
    }
}

void EditorScene::BeforeRebuildUI()
{
    Scene::BeforeRebuildUI();

    auto statsWindows = entitiesRegistry->GetComponentIterator<StatsWindow>();
    for (int i = 0; i < statsWindows.Size(); ++i)
    {
        statsWindows[i].UpdateInfo();
    }
}

void EditorScene::AfterPrepareDraw()
{
    Scene::AfterPrepareDraw();

    auto appViews = entitiesRegistry->GetComponentIterator<AppView>();
    for (int i = 0; i < appViews.Size(); ++i)
    {
        appViews[i].Update(entitiesRegistry);
    }

    auto sceneViews = entitiesRegistry->GetComponentIterator<SceneView>();
    for (int i = 0; i < sceneViews.Size(); ++i)
    {
        sceneViews[i].Update(entitiesRegistry);
    }
}
