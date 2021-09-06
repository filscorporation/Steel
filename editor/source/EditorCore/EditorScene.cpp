#include "EditorScene.h"
#include "../UI/AppView.h"

void EditorScene::AfterPrepareDraw()
{
    Scene::AfterPrepareDraw();

    auto appViews = entitiesRegistry->GetComponentIterator<AppView>();
    int appViewsSize = appViews.Size();
    for (int i = 0; i < appViewsSize; ++i)
    {
        appViews[i].Update(entitiesRegistry);
    }
}
