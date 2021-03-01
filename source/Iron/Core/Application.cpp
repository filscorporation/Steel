#include <iostream>
#include <GLAD/glad.h>
// For windows path
// TODO: make if for linux #include <limits.h> #include <unistd.h>
#include <windows.h>

#include "Application.h"
#include "Log.h"
#include "Time.h"
#include "../Rendering/Renderer.h"
#include "../Audio/AudioSystem.h"
#include "../Physics/PhysicsCore.h"
#include "../Scripting/ScriptingSystem.h"

Application* Application::Instance;

Application::Application(ApplicationSettings settings)
{
    state = ApplicationStates::Initializing;
    Init(settings);

    Instance = this;
}

void Application::Init(ApplicationSettings settings)
{
    ScriptingSystem::Init();

    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen);

    gladLoadGL();

    resources = new ResourcesManager();
    scene = new Scene();

    Renderer::Init(scene->MainCamera);
    AudioSystem::Init(scene->MainCamera->ParentEntity);
    PhysicsCore::Init();

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
    Renderer::OnBeforeRender();

    // Update and render objects in scene
    state = ApplicationStates::OnUpdate;
    auto entities = std::vector<Entity*>(scene->Entities);
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
        entity->OnUpdate();
    }
    state = ApplicationStates::OnPhysicsUpdate;
    // TODO: wrong delta time; physics should run on different thread with fixed delta time
    PhysicsCore::Step(Time::DeltaTime());
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

        for (auto &component : entity->Components())
        {
            try
            {
                component->OnFixedUpdate();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in physics update: " + std::string(ex.what()));
            }
        }
        entity->OnFixedUpdate();
    }

    state = ApplicationStates::OnLateUpdate;
    for (auto &entity : entities)
    {
        for (auto &component : entity->Components())
        {
            try
            {
                component->OnLateUpdate();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in late update: " + std::string(ex.what()));
            }
        }
        entity->OnLateUpdate();
    }

    state = ApplicationStates::OnRender;
    for (auto &entity : entities)
    {
        for (auto &component : entity->Components())
        {
            try
            {
                // TODO: change to render calls and do depth sorting
                component->OnRender();
            }
            catch (const std::exception& ex)
            {
                Log::LogError("Error in render: " + std::string(ex.what()));
            }
        }
    }
    entities.clear();

    //Update scene
    state = ApplicationStates::CleaningDestroyedEntities;
    scene->CleanDestroyedEntities();

    Screen::SwapBuffers();

    Time::Update();

    if (Screen::WindowShouldClose())
        isRunning = false;

    // TODO: testing fps (will be moved to debug UI)
    fpsCounter ++;
    fpsTimer += Time::DeltaTime();
    if (fpsTimer > 1.0f)
    {
        Log::LogInfo("FPS " + std::to_string(fpsCounter));
        Log::LogInfo("Entities in scene " + std::to_string(scene->Entities.size()));
        Log::LogInfo("Screen size " + std::to_string(Screen::GetWidth()) + " " + std::to_string(Screen::GetHeight()));
        fpsTimer = 0;
        fpsCounter = 0;
    }
}

void Application::Terminate()
{
    scene->CleanAllEntities();
    delete scene;

    delete resources;

    PhysicsCore::Terminate();
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
    // Linux
    //int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
    //if(bytes >= 0)
    //    pBuf[bytes] = '\0';
    //return bytes;

    // Windows
    char result[MAX_PATH];
    GetModuleFileName(nullptr, result, MAX_PATH);
    return std::string(result);
}

std::string Application::GetDataPath()
{
    // TODO
    return GetRuntimePath();
}
