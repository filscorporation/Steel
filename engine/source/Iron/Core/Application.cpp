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

    resourcesManager = new ResourcesManager();
    resourcesManager->LoadDefaultResources();

    sceneManager = new SceneManager();
    sceneManager->SetActiveScene(sceneManager->CreateNewScene());
    sceneManager->GetActiveScene()->CreateMainCamera();

    Debug::Init();

    Log::LogDebug("Application initialized");
}

void Application::Run()
{
    Log::LogDebug("Running application");

    isRunning = true;

    ScriptingSystem::CallEntryPoint();

    while (isRunning)
    {
        RunUpdate();
    }

    Terminate();
}

void Application::RunUpdate()
{
    if (!isRunning)
        return; // When updated not from Run()

    Input::PollEvents();

    Screen::UpdateSize();

    // Update scene
    sceneManager->GetActiveScene()->Update();

    // Render scene
    sceneManager->GetActiveScene()->Draw();

    // Update debug info
    //Debug::Update(); TODO

    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        isRunning = false;
}

void Application::Terminate()
{
    Debug::Terminate();

    delete sceneManager;
    delete resourcesManager;

    AudioCore::Terminate();
    Renderer::Terminate();
    Screen::Terminate();
    ScriptingSystem::Terminate();
}

void Application::Quit()
{
    isRunning = false;
}

ResourcesManager* Application::GetResourcesManager()
{
    return resourcesManager;
}

SceneManager* Application::GetSceneManager()
{
    return sceneManager;
}

Scene* Application::GetCurrentScene()
{
    // TODO: probably remove
    return sceneManager->GetActiveScene();
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
    return GetRuntimePath() + "\\" + resourcesManager->GetResourcesPath();
}
