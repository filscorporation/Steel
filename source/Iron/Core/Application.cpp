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
#include "../Audio/AudioSystem.h"
#include "../Physics/Physics.h"
#include "../Rendering/Renderer.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Rendering/SpriteRenderer.h"
#include "../UI/UIRenderer.h"

Application* Application::Instance;

Application::Application(ApplicationSettings settings)
{
    state = ApplicationStates::Initializing;
    Init(settings);

    Instance = this;
}

void Application::Init(ApplicationSettings settings)
{
    Log::LogInfo("Running in " + GetRuntimePath());

    ScriptingSystem::Init();

    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen);

    resources = new ResourcesManager();
    scene = new Scene();

    Renderer::Init(scene->GetMainCamera());
    AudioSystem::Init(scene->GetMainCamera()->ParentEntity);
    Physics::Init();

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
    auto entities = std::list<Entity*>(scene->Entities);
    for (auto &entity : entities)
    {
        for (auto &component : entity->Components())
        {
            try
            {
                component->OnUpdate();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in update: " + std::string(ex.what()));
            }
        }
    }

    state = ApplicationStates::OnPhysicsUpdate;
    // TODO: wrong delta time; physics should run on different thread with fixed delta time
    Physics::Simulate(Time::DeltaTime());
    // TODO: change to physics double sided list of rigid bodies
    for (auto &entity : entities)
    {
        auto rb = entity->GetComponent<RigidBody>();
        if (rb != nullptr)
        {
            try
            {
                rb->GetPhysicsTransformation();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in physics update: " + std::string(ex.what()));
            }
        }

        auto sc = entity->GetComponent<ScriptComponent>();
        if (sc != nullptr)
        {
            try
            {
                sc->OnFixedUpdate();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in fixed update: " + std::string(ex.what()));
            }
        }
    }

    state = ApplicationStates::OnLateUpdate;
    for (auto &entity : entities)
    {
        auto sc = entity->GetComponent<ScriptComponent>();
        if (sc != nullptr)
        {
            try
            {
                sc->OnLateUpdate();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in late update: " + std::string(ex.what()));
            }
        }
        // TODO: this is here just for deferred destruction, rework
        entity->OnLateUpdate();
    }

    Time::Update();
    Renderer::OnBeforeRender();

    state = ApplicationStates::OnRender;
    // Draw sprites
    for (auto &entity : entities)
    {
        auto sr = entity->GetComponent<SpriteRenderer>();
        if (sr != nullptr)
        {
            try
            {
                sr->OnRender();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in render: " + std::string(ex.what()));
            }
        }
    }
    // Clear depth buffer before rendering UI
    Renderer::PrepareUIRender();
    // Draw UI on top
    for (auto &entity : entities)
    {
        auto uir = entity->GetComponent<UIRenderer>();
        if (uir != nullptr)
        {
            try
            {
                uir->OnRender();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in UI render: " + std::string(ex.what()));
            }
        }
    }

    Renderer::OnAfterRender();

    entities.clear();

    //Update scene
    state = ApplicationStates::CleaningDestroyedEntities;
    scene->CleanDestroyedEntities();

    Screen::SwapBuffers();

    if (Screen::WindowShouldClose())
        isRunning = false;

    // TODO: testing fps (will be moved to debug UI)
    fpsCounter ++;
    fpsTimer += Time::DeltaTime();
    if (fpsTimer > 2.0f)
    {
        Log::LogInfo("FPS " + std::to_string(fpsCounter / 2));
        Log::LogInfo("Entities in scene " + std::to_string(scene->Entities.size()));
        fpsTimer = 0;
        fpsCounter = 0;
    }
}

void Application::Terminate()
{
    Log::LogInfo("Entities was created " + std::to_string(Scene::EntitiesWasCreated));

    scene->CleanAllEntities();
    delete scene;

    delete resources;

    Physics::Terminate();
    AudioSystem::Terminate();
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
    // TODO
    return GetRuntimePath();
}
