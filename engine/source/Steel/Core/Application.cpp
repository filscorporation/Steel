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

#include <iostream>
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif
#ifdef PLATFORM_LINUX
#include <climits>
#include <unistd.h>
#endif

#ifdef DISTRIBUTE_BUILD
#ifdef PLATFORM_ANDROID
#define CONFIG_PATH "/application.config"
#else
#define CONFIG_PATH "application.config"
#endif
#else
#define CONFIG_PATH "../../application.config"
#endif

Application* Application::Instance;

ApplicationContext* Application::Context()
{
    return Instance->CurrentContext;
}

Application::Application()
{
#if defined DISTRIBUTE_BUILD && defined PLATFORM_WINDOWS
    FreeConsole();
#endif

    Instance = this;
}

void Application::Init(ApplicationSettings settings)
{
    RootPath = "../../../..";

    InitSystems(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen, settings.DoubleBuffer, settings.VSync);
    AppContext = CreateContext(settings);
    TryLoadSceneOrCreateDefault(AppContext);

    IsInitialized = true;
    Log::LogDebug("Application initialized");
}

void Application::InitSystems(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync)
{
    Log::Init();

    Random::Init();
    Time::Init();
    Screen::Init(width, height, color, fullscreen, doubleBuffer, vSync);
    Input::Init();
    Screen::InitInputHandler();

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
    context->Resources->LoadResources(ResourcesManager::GetEngineResourcesPath());
    context->Resources->LoadResources(ResourcesManager::GetResourcesPath());
    context->Resources->LoadDefaultResources();

    context->Config = new ApplicationConfig();
    SerializationManager::DeserializeConfig(context->Config, GetConfigPath());

    context->Scenes = new SceneManager();

    ScriptingSystem::CreateDomain();
    context->Scripting = true;

    return context;
}

void Application::TryLoadSceneOrCreateDefault(ApplicationContext* context)
{
    Scene* scene = nullptr;

    if (context->Config->StartingScene != NULL_RESOURCE)
    {
        auto sceneData = context->Resources->GetSceneData(context->Config->StartingScene);
        if (sceneData == nullptr)
        {
            Log::LogError("Starting scene ({0}) was not found in resources", context->Config->StartingScene);
        }
        else
        {
            scene = context->Scenes->CreateNewScene(sceneData);
            if (sceneData->FullPath.empty())
                Log::LogError("Starting scene was loaded, but full path is empty. Deserialization is not possible");
            else
                SerializationManager::DeserializeScene(scene, sceneData->FullPath);
        }
    }

    if (scene == nullptr)
    {
        auto sceneData = new SceneData("New scene");
        context->Resources->AddResource(sceneData);
        scene = context->Scenes->CreateNewScene(sceneData);
    }

    context->Scenes->SetActiveScene(scene);
}

void Application::PrepareRun()
{
    assert(IsInitialized);

    IsRunningInternal = true;

    BeforeStartRunLoop();
}

void Application::RunMainLoop()
{
    assert(IsInitialized);

    while (IsRunningInternal)
    {
        RunUpdate();
    }
}

void Application::BeforeStartRunLoop()
{
    // Init all systems and call OnCreated
    AppContext->Scenes->GetActiveScene()->Init(true);
}

void Application::RunUpdate()
{
    assert(IsInitialized);

    if (!IsRunningInternal)
        return; // When updated not from Run()

    // Set scene we will update and render
    SwitchContext(AppContext);

    Input::PollEvents();
    Screen::UpdateSize();

    TrySwitchScene();

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
    assert(IsInitialized);

    SwitchContext(AppContext);
    delete AppContext->Scenes;
    delete AppContext->Resources;
    delete AppContext;
    ScriptingSystem::UnloadDomain();

    AudioCore::Terminate();
    Renderer::Terminate();
    Time::Terminate();
    Screen::Terminate();
    ScriptingSystem::Terminate();
    SerializationManager::Terminate();
    CoreTypeSystem::Terminate();

    Log::LogDebug("Application terminated");
}

void Application::Quit()
{
    IsRunningInternal = false;
}

bool Application::IsRunning()
{
    return IsRunningInternal;
}

const char* Application::GetConfigPath()
{
    return CONFIG_PATH;
}

void Application::SwitchScenes(Scene* newScene)
{
    SceneToSwitch = newScene;
}

void Application::TrySwitchScene()
{
    if (SceneToSwitch == nullptr)
        return;

    // Terminate current  scene
    AppContext->Scenes->DeleteActiveScene();

    // Load new scene
    AppContext->Scenes->SetActiveScene(SceneToSwitch);

    SceneToSwitch->Init(true);
    SceneToSwitch = nullptr;
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
#ifdef PLATFORM_LINUX
    char arg1[20];
    char result[PATH_MAX + 1] = {0};

    sprintf(arg1, "/proc/%d/exe", getpid());
    readlink(arg1, result, 1024);
    return std::string(result);
#endif
#ifdef PLATFORM_WINDOWS
    char result[MAX_PATH];
    GetModuleFileName(nullptr, result, MAX_PATH);
    return std::string(result);
#endif
#ifdef PLATFORM_ANDROID
    return ""; // not supported
#endif
}

std::string Application::GetRelativeRootPath()
{
    return RootPath;
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
