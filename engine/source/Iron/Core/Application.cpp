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
#include "../Audio/AudioListener.h"
#include "../Animation/Animator.h"
#include "../Debug/Debug.h"
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

    state = ApplicationStates::Initializing;
    Init(settings);
}

void Application::Init(ApplicationSettings settings)
{
    Log::LogInfo("Running in " + GetRuntimePath());

    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen);

    resources = new ResourcesManager();
    scene = new Scene();
    scene->CreateMainCamera();

    ScriptingSystem::Init();
    Renderer::Init();
    AudioCore::Init(scene->GetMainCamera().Owner);
    Physics::Init();

    resources->LoadDefaultFont();

    Debug::Init();

    Log::LogInfo("Application initialized");
}

void Application::Run()
{
    Log::LogInfo("Running application");

    isRunning = true;

    state = ApplicationStates::EntryPoint;
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

    state = ApplicationStates::PollEvents;
    Input::PollEvents();

    Screen::UpdateSize();

    Renderer::Clear(Screen::GetColor());

    // Send callbacks for mouse events to affected entities
    Input::SendMouseCallbacks();

    // Update and render objects in scene
    state = ApplicationStates::OnUpdate;

    // Update scripts
    auto scripts = scene->GetEntitiesRegistry()->GetComponentIterator<ScriptComponent>();
    int scriptsSize = scripts.Size();
    for (int i = 0; i < scriptsSize; ++i)
        if (scripts[i].IsAlive()) scripts[i].OnUpdate();

    // Update coroutines
    ScriptingSystem::UpdateCoroutines();

    if (Time::FixedUpdate())
    {
        state = ApplicationStates::OnPhysicsUpdate;
        // Apply transformations to physics objects and then simulate
        Physics::UpdatePhysicsTransformations();
        Physics::Simulate(Time::FixedDeltaTime());

        auto rigidBodies = scene->GetEntitiesRegistry()->GetComponentIterator<RigidBody>();
        int rigidBodiesSize = rigidBodies.Size();
        for (int i = 0; i < rigidBodiesSize; ++i)
            if (rigidBodies[i].IsAlive())
                rigidBodies[i].GetPhysicsTransformation();

        for (int i = 0; i < scriptsSize; ++i)
            if (scripts[i].IsAlive()) scripts[i].OnFixedUpdate();
    }

    state = ApplicationStates::OnLateUpdate;

    for (int i = 0; i < scriptsSize; ++i)
        if (scripts[i].IsAlive()) scripts[i].OnLateUpdate();

    // Update inner components
    auto animators = scene->GetEntitiesRegistry()->GetComponentIterator<Animator>();
    int animatorsSize = animators.Size();
    for (int i = 0; i < animatorsSize; ++i)
        if (animators[i].IsAlive()) animators[i].OnUpdate();

    auto audioSources = scene->GetEntitiesRegistry()->GetComponentIterator<AudioSource>();
    int audioSourcesSize = audioSources.Size();
    for (int i = 0; i < audioSourcesSize; ++i)
        if (audioSources[i].IsAlive()) audioSources[i].OnUpdate();

    auto audioListeners = scene->GetEntitiesRegistry()->GetComponentIterator<AudioListener>();
    int audioListenersSize = audioListeners.Size();
    for (int i = 0; i < audioListenersSize; ++i)
        if (audioListeners[i].IsAlive()) audioListeners[i].OnUpdate();

    // Update debug info
    Debug::Update();

    // Update UI elements
    scene->GetUILayer()->Update();

    // Clean destroyed entities
    state = ApplicationStates::CleaningDestroyedEntities;
    scene->CleanDestroyedEntities();

    // Sort hierarchy from parents to children and then apply transforms
    scene->SortByHierarchy();
    scene->UpdateGlobalTransformation();
    scene->SortByDrawOrder();
    Renderer::OnBeforeRender(scene->GetMainCamera());
    scene->RefreshTransformation();

    Time::Update();

    state = ApplicationStates::OnRender;
    // Draw scene (sprites)
    scene->Draw();
    // Clear depth buffer before rendering UI
    Renderer::PrepareUIRender();
    // Draw UI on top
    scene->GetUILayer()->Draw();
    // Poll UI events
    UIEvent uiEvent = Input::GetUIEvent();
    scene->GetUILayer()->PollEvent(uiEvent);

    Renderer::OnAfterRender();

    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        isRunning = false;
}

void Application::Terminate()
{
    Log::LogInfo("Entities was created " + std::to_string(Scene::EntitiesWasCreated));

    Debug::Terminate();

    delete scene;

    delete resources;

    Physics::Terminate();
    AudioCore::Terminate();
    Renderer::Terminate();
    Screen::Terminate();
    ScriptingSystem::Terminate();
}

void Application::Quit()
{
    isRunning = false;
}

ApplicationStates::ApplicationState Application::State()
{
    return state;
}

ResourcesManager* Application::GetResourcesManager()
{
    return resources;
}

Scene* Application::GetCurrentScene()
{
    return scene;
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
    return GetRuntimePath() + "\\" + resources->GetResourcesPath();
}
