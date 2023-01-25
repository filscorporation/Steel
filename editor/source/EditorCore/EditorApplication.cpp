#include "EditorApplication.h"
#include "EditorScene.h"
#include "EditorBuilder.h"
#include "EditorSceneRenderer.h"
#include "EditorTypeSystem.h"

#include <Steel/Scripting/ScriptingSystem.h>
#include <Steel/Rendering/SceneRenderer.h>
#include <Steel/Serialization/SerializationManager.h>

void EditorApplication::Init(ApplicationSettings settings)
{
    InitSystems(1400, 800, glm::vec3(0.0f, 0.0f, 0.0f), settings.Fullscreen, settings.DoubleBuffer, settings.VSync);
    EditorTypeSystem::Init();

    // Application
    AppContext = new ApplicationContext();
    CurrentContext = AppContext;
    AppContext->ScreenParams.CanResize = false;
    AppContext->ScreenParams.Fullscreen = false;
    AppContext->ScreenParams.Width = settings.ScreenWidth;
    AppContext->ScreenParams.Height = settings.ScreenHeight;
    AppContext->ScreenParams.WidthBackup = settings.ScreenWidth;
    AppContext->ScreenParams.HeightBackup = settings.ScreenHeight;
    AppContext->ScreenParams.AutoResolution = true;
    AppContext->ScreenParams.ResolutionX = settings.ScreenWidth;
    AppContext->ScreenParams.ResolutionY = settings.ScreenHeight;
    AppContext->ScreenParams.OffsetX = 0;
    AppContext->ScreenParams.OffsetY = 0;
    AppContext->ScreenParams.Color = settings.ScreenColor;
    AppContext->ScreenParams.IsDirty = false;

    AppContext->Resources = new ResourcesManager();
    AppContext->Resources->LoadResources(ENGINE_RESOURCES_PATH);
    AppContext->Resources->LoadResources(RESOURCES_PATH);
    AppContext->Resources->LoadDefaultResources();

    AppContext->Config = new ApplicationConfig();
    SerializationManager::DeserializeConfig(AppContext->Config, GetConfigPath());

    AppContext->Scenes = new SceneManager();

    ScriptingSystem::CreateDomain();
    AppContext->Scripting = true;

    TryLoadSceneOrCreateDefault(AppContext);
    if (AppContext->Config->StartingScene == NULL_RESOURCE)
        // Solution until scene creation menu
        AppContext->Scenes->GetActiveScene()->CreateMainCamera();

    // Editor
    EditorContext = new ApplicationContext();
    CurrentContext = EditorContext;

    EditorContext->ScreenParams.CanResize = true;
    EditorContext->ScreenParams.Fullscreen = false;
    EditorContext->ScreenParams.Width = 1400;
    EditorContext->ScreenParams.Height = 800;
    EditorContext->ScreenParams.ResolutionX = 1400;
    EditorContext->ScreenParams.ResolutionY = 800;
    EditorContext->ScreenParams.OffsetX = 0;
    EditorContext->ScreenParams.OffsetY = 0;
    EditorContext->ScreenParams.Color = glm::vec3(0.0f, 0.0f, 0.0f);

    EditorContext->Resources = new ResourcesManager();
    EditorContext->Resources->LoadResources(ENGINE_RESOURCES_PATH);
    EditorContext->Resources->LoadDefaultResources();

    EditorContext->Config = new ApplicationConfig();

    EditorContext->Scenes = new SceneManager();

    EditorContext->Scripting = false;

    auto editorScene = new EditorScene();
    EditorContext->Scenes->SetActiveScene(editorScene);
    editorScene->CreateMainCamera();
    CreateEditorViewCamera();

    ApplicationFramebuffer = new Framebuffer(AppContext->ScreenParams.ResolutionX, AppContext->ScreenParams.ResolutionY);
    OpenedSceneFramebuffer = new Framebuffer(100, 100);

    IsInitialized = true;

    _selectionController = new SelectionController();
    EditorBuilder::BuildLayout(editorScene);
}

void EditorApplication::BeforeStartRunLoop()
{
    // For editor all initialization and OnCreate are after Play button is pressed
}

void EditorApplication::RunUpdate()
{
    if (!IsRunningInternal)
        return; // When updated not from Run()

    SwitchContext(EditorContext);

    Input::PollEvents();
    Screen::UpdateSize();

    // Update editor scene
    auto editorScene = CurrentContext->Scenes->GetActiveScene();
    editorScene->Refresh();
    editorScene->Update();
    editorScene->PrepareDraw();

    // Set scene we will update and render into framebuffer
    SwitchContext(AppContext);

    TrySwitchScene();
    auto openedScene = CurrentContext->Scenes->GetActiveScene();
    openedScene->Refresh();
    if (state == EditorStates::Playing || state == EditorStates::Step)
    {
        openedScene->Update();
        if (state == EditorStates::Step)
            state = EditorStates::Paused;
    }
    openedScene->PrepareDraw();
    SceneRenderer::Draw(openedScene, ApplicationFramebuffer);
    EditorSceneRenderer::DrawToSceneView(openedScene, OpenedSceneFramebuffer, EditorViewCameraEntity);

    // Now render editor
    SwitchContext(EditorContext);
    SceneRenderer::Draw(editorScene, Screen::ScreenFramebuffer());

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunningInternal = false;
}

void EditorApplication::Terminate()
{
    delete _selectionController;

    delete OpenedSceneFramebuffer;
    delete ApplicationFramebuffer;

    SwitchContext(EditorContext);
    delete EditorContext->Scenes;
    delete EditorContext->Resources;
    delete EditorContext;

    delete sceneBackup;

    EditorTypeSystem::Terminate();
    Application::Terminate();
}

EditorStates::EditorState EditorApplication::GetState() const
{
    return state;
}

void EditorApplication::SetState(EditorStates::EditorState newState)
{
    if (state == newState)
        return;

    switch (newState)
    {
        case EditorStates::Stopped:
            EnterEditMode(newState);
            break;
        case EditorStates::Playing:
            if (state == EditorStates::Stopped)
                EnterPlayMode(newState);
            Time::ResetFixedDeltaTime();
            break;
        case EditorStates::Paused:
            if (state == EditorStates::Stopped)
                return; // TODO: support pause from stopped
            break;
        case EditorStates::Step:
            if (state == EditorStates::Stopped)
                return;
            Time::ResetFixedDeltaTime();
            break;
    }

    state = newState;
}

void EditorApplication::EnterPlayMode(EditorStates::EditorState newState)
{
    SwitchContext(AppContext);

    // Cache and terminate edited scene
    Scene* scene = AppContext->Scenes->GetActiveScene();
    SceneData* sceneData = AppContext->Resources->GetSceneData(scene->GetSourceID());
    sceneBackup = SerializationManager::BackupScene(scene, sceneData);
    AppContext->Scenes->DeleteActiveScene();
    ScriptingSystem::UnloadDomain();

    // Change state
    state = newState;

    // Initialize scene again from cached data
    ScriptingSystem::CreateDomain();
    scene = AppContext->Scenes->CreateNewScene(sceneBackup->Data);
    SerializationManager::RestoreScene(sceneBackup, scene);
    AppContext->Scenes->SetActiveScene(scene);

    scene->Init(true);

    SwitchContext(EditorContext);
}

void EditorApplication::EnterEditMode(EditorStates::EditorState newState)
{
    if (sceneBackup == nullptr)
    {
        Log::LogError("No scene to restore from backup");
        return;
    }

    SwitchContext(AppContext);

    // Terminate played scene
    AppContext->Scenes->DeleteActiveScene();
    ScriptingSystem::UnloadDomain();

    // Change state
    state = newState;

    // Initialize scene from cached data
    ScriptingSystem::CreateDomain();
    auto scene = AppContext->Scenes->CreateNewScene(sceneBackup->Data);
    SerializationManager::RestoreScene(sceneBackup, scene);
    AppContext->Scenes->SetActiveScene(scene);

    scene->Init(false);

    SwitchContext(EditorContext);
}

void EditorApplication::LoadSceneToEdit(SceneData* sceneData)
{
    if (GetState() != EditorStates::Stopped)
    {
        Log::LogError("Can't load scene in play mode");
        return;
    }

    SwitchContext(AppContext);

    // Terminate previously edited scene
    AppContext->Scenes->DeleteActiveScene();
    ScriptingSystem::UnloadDomain();

    // Initialize new scene
    ScriptingSystem::CreateDomain();
    auto scene = AppContext->Scenes->CreateNewScene(sceneData);
    SerializationManager::DeserializeScene(scene, sceneData->FullPath);
    AppContext->Scenes->SetActiveScene(scene);

    scene->Init(false);

    SwitchContext(EditorContext);
}

ApplicationContext* EditorApplication::GetAppContext()
{
    return AppContext;
}

void EditorApplication::CreateEditorViewCamera()
{
    EditorViewCameraEntity = EditorContext->Scenes->GetActiveScene()->CreateEntity("Editor view camera", NULL_ENTITY);
    auto& camera = EditorContext->Scenes->GetActiveScene()->GetEntitiesRegistry()->AddComponent<Camera>(EditorViewCameraEntity);
    // We will manually update it's size from scene view
    camera.AutoResize = false;
    camera.SetHeight(3.0f);
    auto& transform = EditorContext->Scenes->GetActiveScene()->GetEntitiesRegistry()->GetComponent<Transformation>(EditorViewCameraEntity);
    transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
}

bool EditorApplication::IsRunning()
{
    return IsRunningInternal && state == EditorStates::Playing;
}

SelectionController* EditorApplication::GetSelectionController() const
{
    return _selectionController;
}
