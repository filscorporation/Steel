#include "EditorSceneManager.h"
#include "EditorApplication.h"

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
    editedScene = SerializationManager::CopyScene(activeScene);
}

void EditorSceneManager::StartTestEditedScene()
{
    if (!isEditMode)
        return;

    isEditMode = false;

    delete activeScene;
    activeScene = SerializationManager::CopyScene(editedScene);
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

void EditorSceneManager::LoadSceneToEdit(Scene* scene)
{
    delete editedScene;
    editedScene = scene;
    editedScene->InitForEdit();
}
