#pragma once

#include "Scene.h"
#include "Steel/Resources/SceneData.h"

class SceneManager
{
public:
    virtual ~SceneManager();

    Scene* CreateNewScene(SceneData* sceneData);
    virtual Scene* GetActiveScene();
    void SetActiveScene(Scene* scene);
    void DeleteActiveScene();

protected:
    Scene* activeScene = nullptr;
};
