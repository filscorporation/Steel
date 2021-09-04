#include "EditorApplication.h"

#undef LoadImage

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
    EditorContext->Scenes->SetActiveScene(EditorContext->Scenes->CreateNewScene(false));

    EditorContext->Scenes->GetActiveScene()->CreateMainCamera();
    ApplicationFramebuffer = new Framebuffer(Screen::GetWidth(), Screen::GetHeight());
}

void EditorApplication::RunUpdate()
{
    if (!IsRunning)
        return; // When updated not from Run()

    // Set scene we will update and render
    CurrentContext = AppContext;

    Input::PollEvents();
    Screen::UpdateSize();

    if (IsPlaying)
        CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->Draw(ApplicationFramebuffer);

    // Now update and render editor scene
    CurrentContext = EditorContext;

    CurrentContext->Scenes->GetActiveScene()->Update();
    CurrentContext->Scenes->GetActiveScene()->Draw(Screen::ScreenFramebuffer());

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

    Application::Terminate();
}
