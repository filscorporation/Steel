#pragma once

#include "../Scene/Scene.h"

class SceneManager
{
public:
    ~SceneManager();

    Scene* CreateNewScene(bool initSystems = true);
    Scene* GetActiveScene();
    void SetActiveScene(Scene* scene);
    Scene* GetScene(const std::string& sceneName);

private:
    Scene* activeScene;
};
