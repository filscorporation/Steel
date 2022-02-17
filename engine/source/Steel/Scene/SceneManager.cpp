#include "SceneManager.h"

SceneManager::~SceneManager()
{
    delete activeScene;
}

Scene* SceneManager::CreateNewScene(const std::string& sceneName)
{
    auto scene = new Scene(sceneName);

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

Scene* SceneManager::GetScene(const std::string& sceneName)
{
    // TODO: not implemented
    return nullptr;
}
