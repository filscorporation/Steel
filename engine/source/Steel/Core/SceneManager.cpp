#include "SceneManager.h"

SceneManager::~SceneManager()
{
    delete activeScene;
}

Scene* SceneManager::CreateNewScene()
{
    auto scene = new Scene();

    return scene;
}

Scene* SceneManager::GetActiveScene()
{
    return activeScene;
}

void SceneManager::SetActiveScene(Scene* scene)
{
    activeScene = scene;
}

Scene* SceneManager::GetScene(const std::string& sceneName)
{
    // TODO: not implemented
    return nullptr;
}
