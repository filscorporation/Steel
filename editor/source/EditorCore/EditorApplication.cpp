#include <Steel/Scripting/ScriptingSystem.h>
#include "EditorApplication.h"
#include "EditorScene.h"
#include "EditorBuilder.h"
#include "EditorSceneManager.h"

void EditorApplication::Init(ApplicationSettings settings)
{
    InitSystems(1400, 800, glm::vec3(0.0f, 0.0f, 0.0f), settings.Fullscreen, settings.DoubleBuffer);

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
    AppContext->Resources->LoadDefaultResources();

    AppContext->Scenes = new EditorSceneManager();
    AppContext->Scenes->SetActiveScene(AppContext->Scenes->CreateNewScene());
    AppContext->Scenes->GetActiveScene()->CreateMainCamera();

    ((EditorSceneManager*)AppContext->Scenes)->EditActiveScene();

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
    EditorContext->Resources->LoadDefaultResources();

    EditorContext->Scenes = new SceneManager();
    auto editorScene = new EditorScene();
    EditorContext->Scenes->SetActiveScene(editorScene);

    ApplicationFramebuffer = new Framebuffer(AppContext->ScreenParams.ResolutionX, AppContext->ScreenParams.ResolutionY);

    EditorContext->Scenes->GetActiveScene()->CreateMainCamera();

    IsInitialized = true;

    EditorBuilder::BuildLayout(editorScene);
}

void EditorApplication::RunUpdate()
{
    if (!IsRunning)
        return; // When updated not from Run()

    SwitchContext(EditorContext);

    Input::PollEvents();
    Screen::UpdateSize();

    // Update editor scene
    CurrentContext->Scenes->GetActiveScene()->Refresh();
    CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();

    // Set scene we will update and render into framebuffer
    SwitchContext(AppContext);

    CurrentContext->Scenes->GetActiveScene()->Refresh();
    if (state == EditorStates::Playing || state == EditorStates::Step)
    {
        CurrentContext->Scenes->GetActiveScene()->Update();
        if (state == EditorStates::Step)
            state = EditorStates::Paused;
    }
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();
    CurrentContext->Scenes->GetActiveScene()->Draw(ApplicationFramebuffer);

    // Now render editor
    SwitchContext(EditorContext);
    CurrentContext->Scenes->GetActiveScene()->Draw(Screen::ScreenFramebuffer());

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunning = false;
}

void EditorApplication::Terminate()
{
    delete ApplicationFramebuffer;

    SwitchContext(EditorContext);
    delete EditorContext->Scenes;
    delete EditorContext->Resources;
    delete EditorContext;

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

    auto sceneManager = (EditorSceneManager*)AppContext->Scenes;

    switch (newState)
    {
        case EditorStates::Stopped:
            sceneManager->EndTestEditedScene();
            break;
        case EditorStates::Playing:
            if (state == EditorStates::Stopped)
            {
                sceneManager->StartTestEditedScene();
                SwitchContext(AppContext);
                ScriptingSystem::CallEntryPoint();
                SwitchContext(EditorContext);
            }
            break;
        case EditorStates::Paused:
            if (state == EditorStates::Stopped)
                return; // TODO: support pause from stopped
            break;
        case EditorStates::Step:
            if (state == EditorStates::Stopped)
                return;
            break;
    }

    state = newState;
}

ApplicationContext* EditorApplication::GetAppContext()
{
    return AppContext;
}
