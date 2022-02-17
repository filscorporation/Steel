#pragma once

#include "Scene.h"

class SceneManager
{
public:
    virtual ~SceneManager();

    Scene* CreateNewScene(const std::string& sceneName);
    virtual Scene* GetActiveScene();
    void SetActiveScene(Scene* scene);
    void DeleteActiveScene();
    Scene* GetScene(const std::string& sceneName);

protected:
    Scene* activeScene = nullptr;
};
