#pragma once

#include <Steel/Core/SceneManager.h>

class EditorSceneManager : public SceneManager
{
public:
    ~EditorSceneManager() override;

    Scene* GetActiveScene() override;

    void EditActiveScene();
    void StartTestEditedScene();
    void EndTestEditedScene();

private:
    Scene* editedScene = nullptr;
    bool isEditMode = false;
};
