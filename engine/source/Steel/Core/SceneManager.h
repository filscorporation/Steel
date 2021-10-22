#pragma once

#include "../Scene/Scene.h"

class SceneManager
{
public:
    virtual ~SceneManager();

    Scene* CreateNewScene();
    virtual Scene* GetActiveScene();
    void SetActiveScene(Scene* scene);
    Scene* GetScene(const std::string& sceneName);

protected:
    Scene* activeScene = nullptr;
};
