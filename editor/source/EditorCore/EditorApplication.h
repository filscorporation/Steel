#pragma once

#include "../UI/SelectionController.h"

#include <Steel.h>
#include <Steel/Scene/SceneBackup.h>

namespace EditorStates
{
    enum EditorState
    {
        Stopped,
        Playing,
        Paused,
        Step,
    };
}

class EditorApplication : public Application
{
public:
    EditorApplication() : Application() {}

    void Init(ApplicationSettings settings) override;
    void BeforeStartRunLoop() override;
    void RunUpdate() override;
    bool IsRunning() override;

    EditorStates::EditorState GetState() const;
    void SetState(EditorStates::EditorState newState);
    void EnterPlayMode(EditorStates::EditorState newState);
    void EnterEditMode(EditorStates::EditorState newState);
    void LoadSceneToEdit(SceneData* sceneData);

    ApplicationContext* GetAppContext();
    Framebuffer* ApplicationFramebuffer = nullptr;
    Framebuffer* OpenedSceneFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;
    EntityID EditorViewCameraEntity = NULL_ENTITY;

    SelectionController* GetSelectionController() const;

private:
    void Terminate() override;
    void CreateEditorViewCamera();

    EditorStates::EditorState state = EditorStates::Stopped;
    SceneBackup* sceneBackup = nullptr;
    SelectionController* _selectionController = nullptr;

    friend class AppView;
    friend class ControlPanel;
    friend class HierarchyView;
    friend class ProjectView;
    friend class PropertiesView;
    friend class SceneView;
};
