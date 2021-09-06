#include "EditorApplication.h"
#include "EditorScene.h"
#include "EditorBuilder.h"

EditorApplication::EditorApplication(ApplicationSettings settings) : Application(settings)
{
    Init();
}

void EditorApplication::Init()
{
    EditorContext = new ApplicationContext();
    CurrentContext = EditorContext;

    EditorContext->Resources = new ResourcesManager();
    EditorContext->Resources->LoadDefaultResources();

    EditorContext->Scenes = new SceneManager();
    auto editorScene = new EditorScene();
    EditorContext->Scenes->SetActiveScene(editorScene);

    EditorContext->Scenes->GetActiveScene()->CreateMainCamera();
    ApplicationFramebuffer = new Framebuffer(Screen::GetWidth(), Screen::GetHeight());
    ApplicationWidth = Screen::GetWidth();
    ApplicationHeight = Screen::GetHeight();

    EditorBuilder::BuildLayout(editorScene);
}

void EditorApplication::RunUpdate()
{
    if (!IsRunning)
        return; // When updated not from Run()

    Input::PollEvents();
    Screen::UpdateSize();

    // Update editor scene
    CurrentContext = EditorContext;
    CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();

    // Set scene we will update and render into framebuffer
    CurrentContext = AppContext;

    Screen::StartEmulate(ApplicationWidth, ApplicationHeight);
    Input::InputOffset = { ApplicationX, ApplicationY };
    Input::InputWindowSize = { ApplicationWindowWidth, ApplicationWindowHeight };

    if (IsPlaying)
    {
        CurrentContext->Scenes->GetActiveScene()->Update();
    }
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();
    CurrentContext->Scenes->GetActiveScene()->Draw(ApplicationFramebuffer);

    Screen::StopEmulate();
    Input::InputOffset = { 0.0f, 0.0f };
    Input::InputWindowSize = { Screen::GetWidth(), Screen::GetHeight() };

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
    CurrentContext = EditorContext;
    delete EditorContext->Scenes;
    delete EditorContext->Resources;
    delete EditorContext;

    delete ApplicationFramebuffer;

    Application::Terminate();
}
