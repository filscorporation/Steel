#include <iostream>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#ifdef __unix__
#include <climits>
#include <unistd.h>
#endif

#include "Application.h"
#include "Log.h"
#include "Time.h"
#include "Steel/Audio/AudioCore.h"
#include "Steel/EntityComponentSystem/CoreTypeSystem.h"
#include "Steel/Input/Input.h"
#include "Steel/Math/Random.h"
#include "Steel/Physics/Physics.h"
#include "Steel/Rendering/Renderer.h"
#include "Steel/Rendering/SceneRenderer.h"
#include "Steel/Scripting/ScriptingSystem.h"
#include "Steel/Serialization/SerializationManager.h"

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

    IsInitialized = true;
    Log::LogDebug("Application initialized");
}

void Application::InitSystems(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer)
{
    Log::Init();

    Random::Init();
    Screen::Init(width, height, color, fullscreen, doubleBuffer);

    CoreTypeSystem::Init();
    SerializationManager::Init();
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
    context->Resources->SetDefaultPixelsPerUnit(settings.DefaultPixelsPerUnit);
    context->Resources->LoadResources();
    context->Resources->LoadDefaultResources();

    context->Scenes = new SceneManager();
    context->Scenes->SetActiveScene(context->Scenes->CreateNewScene("New scene"));
    context->Scenes->GetActiveScene()->Init(true);

    ScriptingSystem::CreateDomain();
    context->Scripting = true;

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

    IsRunningInternal = true;

    while (IsRunningInternal)
    {
        RunUpdate();
    }

    Terminate();
}

void Application::RunUpdate()
{
    if (!IsRunningInternal)
        return; // When updated not from Run()

    // Set scene we will update and render
    SwitchContext(AppContext);

    if (!EntryPointCalled)
    {
        EntryPointCalled = true;
        if (CurrentContext->Scripting)
            ScriptingSystem::CallEntryPoint();
    }

    Input::PollEvents();
    Screen::UpdateSize();

    // Update scene
    auto scene = CurrentContext->Scenes->GetActiveScene();
    scene->Refresh();
    scene->Update();

    // Prepare scene to rendering (update components after changing positions and other properties)
    scene->PrepareDraw();

    // Render scene
    SceneRenderer::Draw(scene, Screen::ScreenFramebuffer());

    Time::Update();
    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        IsRunningInternal = false;
}

void Application::Terminate()
{
    SwitchContext(AppContext);
    delete AppContext->Scenes;
    delete AppContext->Resources;
    delete AppContext;
    ScriptingSystem::UnloadDomain();

    AudioCore::Terminate();
    Renderer::Terminate();
    Screen::Terminate();
    ScriptingSystem::Terminate();
    SerializationManager::Terminate();
    CoreTypeSystem::Terminate();
}

void Application::Quit()
{
    IsRunningInternal = false;
}

bool Application::IsRunning()
{
    return IsRunningInternal;
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
