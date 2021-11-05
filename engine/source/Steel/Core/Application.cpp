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
#include "../Math/Random.h"
#include "../Physics/Physics.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/SceneRenderer.h"
#include "../Scripting/ScriptingSystem.h"

Application* Application::Instance;

ApplicationContext* Application::Context()
{
    return Instance->CurrentContext;
}

Application::Application()
{
#ifdef DISTRIBUTE_BUILD
    FreeConsole();
#endif

    Instance = this;
}

void Application::Init(ApplicationSettings settings)
{
    InitSystems(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen, settings.DoubleBuffer);
    AppContext = CreateContext(settings);
    AppContext->Scenes->GetActiveScene()->CreateMainCamera();
    ScriptingSystem::CallEntryPoint();

    IsInitialized = true;
    Log::LogDebug("Application initialized");
}

void Application::InitSystems(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer)
{
    Log::Init();

    Random::Init();
    Screen::Init(width, height, color, fullscreen, doubleBuffer);

    ScriptingSystem::Init();
    Renderer::Init();
    AudioCore::Init();
}

ApplicationContext* Application::CreateContext(ApplicationSettings settings)
{
    auto context = new ApplicationContext();
    CurrentContext = context;

    context->ScreenParams.CanResize = true;
    context->ScreenParams.Fullscreen = settings.Fullscreen;
    context->ScreenParams.Width = settings.ScreenWidth;
    context->ScreenParams.Height = settings.ScreenHeight;
    context->ScreenParams.WidthBackup = settings.ScreenWidth;
    context->ScreenParams.HeightBackup = settings.ScreenHeight;
    context->ScreenParams.AutoResolution = true;
    context->ScreenParams.ResolutionX = settings.ScreenWidth;
    context->ScreenParams.ResolutionY = settings.ScreenHeight;
    context->ScreenParams.OffsetX = 0;
    context->ScreenParams.OffsetY = 0;
    context->ScreenParams.Color = settings.ScreenColor;
    context->ScreenParams.IsDirty = false;

    context->Resources = new ResourcesManager();
    context->Resources->LoadDefaultResources();

    context->Scenes = new SceneManager();
    context->Scenes->SetActiveScene(context->Scenes->CreateNewScene());

    return context;
}

void Application::Run()
{
    if (!IsInitialized)
    {
        std::cout << "ERROR: APPLICATION IS NOT INITIALIZED" << std::endl;
        return;
    }

    Log::LogDebug("Running application");

    IsRunning = true;

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
    SwitchContext(AppContext);

    Input::PollEvents();
    Screen::UpdateSize();

    // Update scene
    auto scene = CurrentContext->Scenes->GetActiveScene();
    scene->Refresh();
    scene->Update();

    // Render scene
    scene->PrepareDraw();
    SceneRenderer sceneRenderer(scene, Screen::ScreenFramebuffer(), scene->GetMainCamera());
    sceneRenderer.DrawScene();

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunning = false;
}

void Application::Terminate()
{
    SwitchContext(AppContext);
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

ScreenParameters& Application::ScreenParametersForUpdate()
{
    return CurrentContext->ScreenParams;
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

void Application::SwitchContext(ApplicationContext* context)
{
    CurrentContext = context;
    if (CurrentContext->ScreenParams.IsDirty)
    {
        Screen::SetScreenSizeDirty();
        Screen::EnterCallback();
        Screen::Apply();
        Screen::ExitCallback();
        CurrentContext->ScreenParams.IsDirty = false;
    }
}
