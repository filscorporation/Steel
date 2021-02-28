#include <iostream>
#include <GLAD/glad.h>

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

    float timer = 0;
    int counter = 0;

    ScriptingSystem::CallEntryPoint();

    while (isRunning)
    {
        Input::PollEvents();

        Renderer::Clear(Screen::Color());
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

        // TODO: testing fps
        counter ++;
        timer += Time::DeltaTime();
        if (timer > 1.0f)
        {
            Log::LogInfo("FPS " + std::to_string(counter));
            Log::LogInfo("Entities in scene " + std::to_string(scene->Entities.size()));
            timer = 0;
            counter = 0;
        }
    }

    Terminate();
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

ResourcesManager *Application::GetResourcesManager()
{
    return resources;
}

Scene* Application::GetCurrentScene()
{
    return scene;
}
