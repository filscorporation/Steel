#include "EditorSceneManager.h"

EditorSceneManager::~EditorSceneManager()
{
    delete editedScene;
}

Scene* EditorSceneManager::GetActiveScene()
{
    return isEditMode ? editedScene : activeScene;
}

void EditorSceneManager::EditActiveScene()
{
    isEditMode = true;

    delete editedScene;
    editedScene = new Scene(*activeScene);
}

void EditorSceneManager::StartTestEditedScene()
{
    if (!isEditMode)
        return;

    isEditMode = false;

    delete activeScene;
    activeScene = new Scene(*editedScene);
    activeScene->Init();
}

void EditorSceneManager::EndTestEditedScene()
{
    if (isEditMode)
        return;

    isEditMode = true;

    delete activeScene;
    activeScene = nullptr;
}
