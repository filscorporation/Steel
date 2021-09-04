#include "SceneManager.h"

SceneManager::~SceneManager()
{
    delete activeScene;
}

Scene* SceneManager::CreateNewScene(bool initSystems)
{
    auto scene = new Scene(initSystems);

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
