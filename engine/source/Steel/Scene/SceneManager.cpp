#include "SceneManager.h"

SceneManager::~SceneManager()
{
    delete activeScene;
}

Scene* SceneManager::CreateNewScene(SceneData* sceneData)
{
    auto scene = new Scene(sceneData->GetName(), sceneData->ID);

    return scene;
}

Scene* SceneManager::GetActiveScene()
{
    return activeScene;
}

void SceneManager::SetActiveScene(Scene* scene)
{
    delete activeScene;
    activeScene = scene;
}

void SceneManager::DeleteActiveScene()
{
    delete activeScene;
    activeScene = nullptr;
}
