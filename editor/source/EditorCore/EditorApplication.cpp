#include "EditorApplication.h"
#include "EditorScene.h"
#include "EditorBuilder.h"

void EditorApplication::Init(ApplicationSettings settings)
{
    InitSystems(1400, 800, glm::vec3(0.0f, 0.0f, 0.0f), settings.Fullscreen, settings.DoubleBuffer);

    // Application
    AppContext = CreateContext(settings);
    AppContext->ScreenParams.CanResize = false;
    AppContext->ScreenParams.Fullscreen = false;
    AppContext->Scenes->GetActiveScene()->CreateMainCamera();

    // Editor
    State = EditorStates::Playing;

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
    CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();

    // Set scene we will update and render into framebuffer
    SwitchContext(AppContext);

    if (State == EditorStates::Playing || State == EditorStates::Step)
    {
        CurrentContext->Scenes->GetActiveScene()->Update();
        if (State == EditorStates::Step)
            State = EditorStates::Paused;
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

ApplicationContext* EditorApplication::GetAppContext()
{
    return AppContext;
}
