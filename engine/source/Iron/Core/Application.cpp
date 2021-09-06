#include <iostream>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#ifdef __unix__
#include <climits>
#include <unistd.h>
#endif

#include "Application.h"
#include "Input.h"
#include "Log.h"
#include "Time.h"
#include "../Audio/AudioCore.h"
#include "../Animation/Animator.h"
#include "../Debug/Debug.h"
#include "../Math/Random.h"
#include "../Physics/Physics.h"
#include "../Rendering/Renderer.h"
#include "../Scripting/ScriptingSystem.h"

Application* Application::Instance;

Application::Application(ApplicationSettings settings)
{
#ifdef DISTRIBUTE_BUILD
    FreeConsole();
#endif

    Instance = this;

    Init(settings);
}

void Application::Init(ApplicationSettings settings)
{
    Log::Init();
    Log::LogDebug("Running in {0}", GetRuntimePath());

    Random::Init();
    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen, settings.DoubleBuffer);

    ScriptingSystem::Init();
    Renderer::Init();
    AudioCore::Init();

    AppContext = new ApplicationContext();
    CurrentContext = AppContext;

    AppContext->Resources = new ResourcesManager();
    AppContext->Resources->LoadDefaultResources();

    AppContext->Scenes = new SceneManager();
    AppContext->Scenes->SetActiveScene(AppContext->Scenes->CreateNewScene());

    AppContext->Scenes->GetActiveScene()->CreateMainCamera();

    Log::LogDebug("Application initialized");
}

void Application::Run()
{
    Log::LogDebug("Running application");

    IsRunning = true;

    CurrentContext = AppContext;
    ScriptingSystem::CallEntryPoint();

    while (IsRunning)
    {
        RunUpdate();
    }

    Terminate();
}

void Application::RunUpdate()
{
    if (!IsRunning)
        return; // When updated not from Run()

    // Set scene we will update and render
    CurrentContext = AppContext;

    Input::PollEvents();
    Screen::UpdateSize();

    // Update scene
    CurrentContext->Scenes->GetActiveScene()->Update();

    // Render scene
    CurrentContext->Scenes->GetActiveScene()->PrepareDraw();
    CurrentContext->Scenes->GetActiveScene()->Draw(Screen::ScreenFramebuffer());

    Input::InputOffset = { 0.0f, 0.0f };
    Input::InputWindowSize = {Screen::GetWidth(), Screen::GetHeight() };

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunning = false;
}

void Application::Terminate()
{
    CurrentContext = AppContext;
    delete AppContext->Scenes;
    delete AppContext->Resources;
    delete AppContext;

    AudioCore::Terminate();
    Renderer::Terminate();
    Screen::Terminate();
    ScriptingSystem::Terminate();
}

void Application::Quit()
{
    IsRunning = false;
}

ResourcesManager* Application::GetResourcesManager()
{
    return CurrentContext->Resources;
}

SceneManager* Application::GetSceneManager()
{
    return CurrentContext->Scenes;
}

Scene* Application::GetCurrentScene()
{
    return CurrentContext->Scenes->GetActiveScene();
}

std::string Application::GetRuntimePath()
{
#ifdef __unix__
    char arg1[20];
    char result[PATH_MAX + 1] = {0};

    sprintf(arg1, "/proc/%d/exe", getpid());
    readlink(arg1, result, 1024);
    return std::string(result);
#endif
#if defined(_WIN32) || defined(_WIN64)
    char result[MAX_PATH];
    GetModuleFileName(nullptr, result, MAX_PATH);
    return std::string(result);
#endif
}

std::string Application::GetDataPath()
{
    return GetRuntimePath() + "\\" + AppContext->Resources->GetResourcesPath();
}
