#pragma once

#include <Steel/Scene/SceneManager.h>

class EditorSceneManager : public SceneManager
{
public:
    ~EditorSceneManager() override;

    Scene* GetActiveScene() override;

    void EditActiveScene();
    void StartTestEditedScene();
    void EndTestEditedScene();
    void LoadSceneToEdit(Scene* scene);

private:
    Scene* editedScene = nullptr;
    bool isEditMode = false;
};
