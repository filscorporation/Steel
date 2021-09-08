#include "EditorApplication.h"
#include "EditorScene.h"
#include "EditorBuilder.h"

void EditorApplication::Init(ApplicationSettings settings)
{
    InitSystems(1400, 800, glm::vec3(0.0f, 0.0f, 0.0f), settings.Fullscreen, settings.DoubleBuffer);

    // Application
    AppContext = CreateContext(settings);
    AppContext->ScreenParameters.CanResize = false;
    AppContext->ScreenParameters.Fullscreen = false;
    AppContext->Scenes->GetActiveScene()->CreateMainCamera();

    // Editor
    State = EditorStates::Playing;

    EditorContext = new ApplicationContext();
    CurrentContext = EditorContext;

    EditorContext->ScreenParameters.CanResize = true;
    EditorContext->ScreenParameters.Fullscreen = false;
    EditorContext->ScreenParameters.Width = 1400;
    EditorContext->ScreenParameters.Height = 800;
    EditorContext->ScreenParameters.ResolutionX = 1400;
    EditorContext->ScreenParameters.ResolutionY = 800;
    EditorContext->ScreenParameters.OffsetX = 0;
    EditorContext->ScreenParameters.OffsetY = 0;
    EditorContext->ScreenParameters.Color = glm::vec3(0.0f, 0.0f, 0.0f);

    EditorContext->Resources = new ResourcesManager();
    EditorContext->Resources->LoadDefaultResources();

    EditorContext->Scenes = new SceneManager();
    auto editorScene = new EditorScene();
    EditorContext->Scenes->SetActiveScene(editorScene);

    ApplicationFramebuffer = new Framebuffer(AppContext->ScreenParameters.ResolutionX, AppContext->ScreenParameters.ResolutionY);

    EditorContext->Scenes->GetActiveScene()->CreateMainCamera();

    IsInitialized = true;

    EditorBuilder::BuildLayout(editorScene);
}

void EditorApplication::RunUpdate()
{
    if (!IsRunning)
        return; // When updated not from Run()

    CurrentContext = EditorContext;

    Input::PollEvents();
    Screen::UpdateSize();

    // Update editor scene
    CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();

    // Set scene we will update and render into framebuffer
    CurrentContext = AppContext;

    if (State == EditorStates::Playing || State == EditorStates::Step)
    {
        CurrentContext->Scenes->GetActiveScene()->Update();
        if (State == EditorStates::Step)
            State = EditorStates::Paused;
    }
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();
    CurrentContext->Scenes->GetActiveScene()->Draw(ApplicationFramebuffer);

    // Now render editor
    CurrentContext = EditorContext;
    CurrentContext->Scenes->GetActiveScene()->Draw(Screen::ScreenFramebuffer());

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunning = false;
}

void EditorApplication::Terminate()
{
    delete ApplicationFramebuffer;

    CurrentContext = EditorContext;
    delete EditorContext->Scenes;
    delete EditorContext->Resources;
    delete EditorContext;

    Application::Terminate();
}

ApplicationContext* EditorApplication::GetAppContext()
{
    return AppContext;
}
